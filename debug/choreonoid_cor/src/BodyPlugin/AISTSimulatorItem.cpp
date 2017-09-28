/*!
  @file
  @author Shin'ichiro Nakaoka
*/
#ifdef WIN32
#include <boost/version.hpp>
#if (BOOST_VERSION >= 105900) 
#define BOOST_NO_CXX11_ALLOCATOR
#endif
#endif

#include "AISTSimulatorItem.h"
#include "BodyItem.h"
#include "ControllerItem.h"
#include <cnoid/ItemManager>
#include <cnoid/Archive>
#include <cnoid/EigenArchive>
#include <cnoid/DyWorld>
#include <cnoid/DyBody>
#include <cnoid/ForwardDynamicsCBM>
#include <cnoid/ConstraintForceSolver>
#include <cnoid/LeggedBodyHelper>
#include <cnoid/FloatingNumberString>
#include <cnoid/EigenUtil>
#include <cnoid/MessageView>
#include <cnoid/IdPair>
#include <boost/lexical_cast.hpp>
#include <mutex>
#include <iostream>
#include <iomanip>
#include "gettext.h"

using namespace std;
using namespace std::placeholders;
using namespace cnoid;
using boost::format;

// for Windows
#undef min
#undef max

namespace {

const bool TRACE_FUNCTIONS = false;
const bool ENABLE_DEBUG_OUTPUT = false;
const double DEFAULT_GRAVITY_ACCELERATION = 9.80665;

class AISTSimBody : public SimulationBody
{
public:
    AISTSimBody(DyBody* body) : SimulationBody(body) { }
};
    

class KinematicWalkBody : public AISTSimBody
{
public:
    KinematicWalkBody(DyBody* body, LeggedBodyHelper* legged)
        : AISTSimBody(body),
          legged(legged) {
        supportFootIndex = 0;
        for(int i=1; i < legged->numFeet(); ++i){
            if(legged->footLink(i)->p().z() < legged->footLink(supportFootIndex)->p().z()){
                supportFootIndex = i;
            }
        }
        traverse.find(legged->footLink(supportFootIndex), true, true);
    }
    LeggedBodyHelper* legged;
    int supportFootIndex;
    LinkTraverse traverse;
};

}


namespace cnoid {
  
class AISTSimulatorItemImpl
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    AISTSimulatorItem* self;

    World<ConstraintForceSolver> world;
        
    Selection dynamicsMode;
    Selection integrationMode;
    Vector3 gravity;
    double staticFriction;
    double dynamicFriction;
    FloatingNumberString contactCullingDistance;
    FloatingNumberString contactCullingDepth;
    FloatingNumberString errorCriterion;
    int maxNumIterations;
    FloatingNumberString contactCorrectionDepth;
    FloatingNumberString contactCorrectionVelocityRatio;
    double epsilon;
    bool is2Dmode;
    bool isKinematicWalkingEnabled;
    bool isOldAccelSensorMode;

    typedef std::map<Body*, int> BodyIndexMap;
    BodyIndexMap bodyIndexMap;

    typedef std::map<Link*, Link*> LinkMap;
    LinkMap orgLinkToInternalLinkMap;

    struct ContactAttribute
    {
        boost::optional<double> staticFriction;
        boost::optional<double> dynamicFriction;
        boost::optional<int> collisionHandlerId;
    };

    typedef std::map<IdPair<Link*>, ContactAttribute> ContactAttributeMap;
    ContactAttributeMap contactAttributeMap;

    boost::optional<int> forcedBodyPositionFunctionId;
    std::mutex forcedBodyPositionMutex;
    DyBody* forcedPositionBody;
    Position forcedBodyPosition;

    AISTSimulatorItemImpl(AISTSimulatorItem* self);
    AISTSimulatorItemImpl(AISTSimulatorItem* self, const AISTSimulatorItemImpl& org);
    void setFriction(Link* link1, Link* link2, double staticFriction, double slipFriction);
    void setEpsilon(Link* link1, Link* link2, double epsilon);
    ContactAttribute& getOrCreateContactAttribute(Link* link1, Link* link2);
    bool initializeSimulation(const std::vector<SimulationBody*>& simBodies);
    void addBody(AISTSimBody* simBody);
    void clearExternalForces();
    void setForcedPosition(BodyItem* bodyItem, const Position& T);
    void doSetForcedPosition();
    void doPutProperties(PutPropertyFunction& putProperty);
    void addExtraJoint(ExtraJoint& extrajoint);
    void clearExtraJoint();
    bool store(Archive& archive);
    bool restore(const Archive& archive);

    // for debug
    ofstream os;
};

}


void AISTSimulatorItem::initializeClass(ExtensionManager* ext)
{
    ext->itemManager().registerClass<AISTSimulatorItem>(N_("AISTSimulatorItem"));
    ext->itemManager().addCreationPanel<AISTSimulatorItem>();
}


AISTSimulatorItem::AISTSimulatorItem()
{
    impl = new AISTSimulatorItemImpl(this);
    setName("AISTSimulator");
}


AISTSimulatorItemImpl::AISTSimulatorItemImpl(AISTSimulatorItem* self)
    : self(self),
      dynamicsMode(AISTSimulatorItem::N_DYNAMICS_MODES, CNOID_GETTEXT_DOMAIN_NAME),
      integrationMode(AISTSimulatorItem::N_INTEGRATION_MODES, CNOID_GETTEXT_DOMAIN_NAME)
{
    dynamicsMode.setSymbol(AISTSimulatorItem::FORWARD_DYNAMICS,  N_("Forward dynamics"));
    dynamicsMode.setSymbol(AISTSimulatorItem::HG_DYNAMICS,       N_("High-gain dynamics"));
    dynamicsMode.setSymbol(AISTSimulatorItem::KINEMATICS,        N_("Kinematics"));

    integrationMode.setSymbol(AISTSimulatorItem::EULER_INTEGRATION,  N_("Euler"));
    integrationMode.setSymbol(AISTSimulatorItem::RUNGE_KUTTA_INTEGRATION,  N_("Runge Kutta"));
    integrationMode.select(AISTSimulatorItem::RUNGE_KUTTA_INTEGRATION);
    
    gravity << 0.0, 0.0, -DEFAULT_GRAVITY_ACCELERATION;

    ConstraintForceSolver& cfs = world.constraintForceSolver;
    staticFriction = cfs.staticFriction();
    dynamicFriction = cfs.slipFriction();
    contactCullingDistance = cfs.contactCullingDistance();
    contactCullingDepth = cfs.contactCullingDepth();
    epsilon = cfs.coefficientOfRestitution();
    
    errorCriterion = cfs.gaussSeidelErrorCriterion();
    maxNumIterations = cfs.gaussSeidelMaxNumIterations();
    contactCorrectionDepth = cfs.contactCorrectionDepth();
    contactCorrectionVelocityRatio = cfs.contactCorrectionVelocityRatio();

    isKinematicWalkingEnabled = false;
    is2Dmode = false;
    isOldAccelSensorMode = false;
}


AISTSimulatorItem::AISTSimulatorItem(const AISTSimulatorItem& org)
    : SimulatorItem(org),
      impl(new AISTSimulatorItemImpl(this, *org.impl))
{

}


AISTSimulatorItemImpl::AISTSimulatorItemImpl(AISTSimulatorItem* self, const AISTSimulatorItemImpl& org)
    : self(self),
      dynamicsMode(org.dynamicsMode),
      integrationMode(org.integrationMode)
{
    gravity = org.gravity;
    staticFriction = org.staticFriction;
    dynamicFriction = org.dynamicFriction;
    contactCullingDistance = org.contactCullingDistance;
    contactCullingDepth = org.contactCullingDepth;
    errorCriterion = org.errorCriterion;
    maxNumIterations = org.maxNumIterations;
    contactCorrectionDepth = org.contactCorrectionDepth;
    contactCorrectionVelocityRatio = org.contactCorrectionVelocityRatio;
    epsilon = org.epsilon;
    isKinematicWalkingEnabled = org.isKinematicWalkingEnabled;
    is2Dmode = org.is2Dmode;
    isOldAccelSensorMode = org.isOldAccelSensorMode;
}


AISTSimulatorItem::~AISTSimulatorItem()
{
    delete impl;
}


void AISTSimulatorItem::setDynamicsMode(int mode)
{
    impl->dynamicsMode.select(mode);
}


void AISTSimulatorItem::setIntegrationMode(int mode)
{
    impl->integrationMode.select(mode);
}


void AISTSimulatorItem::setGravity(const Vector3& gravity)
{
    impl->gravity = gravity;
}


const Vector3& AISTSimulatorItem::gravity() const
{
    return impl->gravity;
}


AISTSimulatorItemImpl::ContactAttribute&
AISTSimulatorItemImpl::getOrCreateContactAttribute(Link* link1, Link* link2)
{
    return contactAttributeMap[IdPair<Link*>(link1, link2)];
}
        

void AISTSimulatorItem::setFriction(double staticFriction, double dynamicFriction)
{
    impl->staticFriction = staticFriction;
    impl->dynamicFriction = dynamicFriction;
}


double AISTSimulatorItem::staticFriction() const
{
    return impl->staticFriction;
}


double AISTSimulatorItem::dynamicFriction() const
{
    return impl->dynamicFriction;
}


void AISTSimulatorItem::setFriction(Link* link1, Link* link2, double staticFriction, double dynamicFriction)
{
    AISTSimulatorItemImpl::ContactAttribute& attr = impl->getOrCreateContactAttribute(link1, link2);
    attr.staticFriction = staticFriction;
    attr.dynamicFriction = dynamicFriction;
}


int AISTSimulatorItem::registerCollisionHandler(const std::string& name, CollisionHandler handler)
{
    return impl->world.constraintForceSolver.registerCollisionHandler(name, handler);
}


void AISTSimulatorItem::unregisterCollisionHandler(int handlerId)
{
    return impl->world.constraintForceSolver.unregisterCollisionHandler(handlerId);
}


int AISTSimulatorItem::collisionHandlerId(const std::string& name) const
{
    return impl->world.constraintForceSolver.collisionHandlerId(name);
}


void AISTSimulatorItem::setCollisionHandler(Link* link1, Link* link2, int handlerId)
{
    AISTSimulatorItemImpl::ContactAttribute& attr = impl->getOrCreateContactAttribute(link1, link2);
    attr.collisionHandlerId = handlerId;
}


void AISTSimulatorItem::setContactCullingDistance(double value)    
{
    impl->contactCullingDistance = value;
}


void AISTSimulatorItem::setContactCullingDepth(double value)    
{
    impl->contactCullingDepth = value;
}

    
void AISTSimulatorItem::setErrorCriterion(double value)    
{
    impl->errorCriterion = value;
}

    
void AISTSimulatorItem::setMaxNumIterations(int value)
{
    impl->maxNumIterations = value;   
}


void AISTSimulatorItem::setContactCorrectionDepth(double value)
{
    impl->contactCorrectionDepth = value;
}


void AISTSimulatorItem::setContactCorrectionVelocityRatio(double value)
{
    impl->contactCorrectionVelocityRatio = value;
}


void AISTSimulatorItem::setEpsilon(double epsilon)
{
    impl->epsilon = epsilon;
}


void AISTSimulatorItemImpl::setEpsilon(Link* link1, Link* link2, double epsilon)
{
    ContactAttribute& attr = contactAttributeMap[IdPair<Link*>(link1, link2)];
    attr.epsilon = epsilon;
}


void AISTSimulatorItem::setEpsilon(Link* link1, Link* link2, double epsilon)
{
    impl->setEpsilon(link1, link2, epsilon);
}


void AISTSimulatorItem::set2Dmode(bool on)
{
    impl->is2Dmode = on;
}


void AISTSimulatorItem::setKinematicWalkingEnabled(bool on)
{
    impl->isKinematicWalkingEnabled = on;
}


void AISTSimulatorItem::setConstraintForceOutputEnabled(bool on)
{
    impl->world.constraintForceSolver.enableConstraintForceOutput(on);
}


Item* AISTSimulatorItem::doDuplicate() const
{
    return new AISTSimulatorItem(*this);
}


bool AISTSimulatorItem::startSimulation(bool doReset)
{
    impl->orgLinkToInternalLinkMap.clear();
    return SimulatorItem::startSimulation(doReset);
}


SimulationBody* AISTSimulatorItem::createSimulationBody(Body* orgBody)
{
    SimulationBody* simBody = 0;
    DyBody* body = new DyBody(*orgBody);

    const int n = orgBody->numLinks();
    for(size_t i=0; i < n; ++i){
        impl->orgLinkToInternalLinkMap[orgBody->link(i)] = body->link(i);
    }
    
    if(impl->dynamicsMode.is(KINEMATICS) && impl->isKinematicWalkingEnabled){
        LeggedBodyHelper* legged = getLeggedBodyHelper(body);
        if(legged->isValid()){
            simBody = new KinematicWalkBody(body, legged);
        }
    }
    if(!simBody){
        simBody = new AISTSimBody(body);
    }

    return simBody;
}


bool AISTSimulatorItem::initializeSimulation(const std::vector<SimulationBody*>& simBodies)
{
    return impl->initializeSimulation(simBodies);
}


bool AISTSimulatorItemImpl::initializeSimulation(const std::vector<SimulationBody*>& simBodies)
{
    if(ENABLE_DEBUG_OUTPUT){
        static int ntest = 0;
        os.open((string("test-log-") + boost::lexical_cast<string>(ntest++) + ".log").c_str());
        os << setprecision(30);
    }

    if(integrationMode.is(AISTSimulatorItem::EULER_INTEGRATION)){
        world.setEulerMethod();
    } else if(integrationMode.is(AISTSimulatorItem::RUNGE_KUTTA_INTEGRATION)){
        world.setRungeKuttaMethod();
    }
    world.setGravityAcceleration(gravity);
    world.enableSensors(true);
    world.setOldAccelSensorCalcMode(isOldAccelSensorMode);
    world.setTimeStep(self->worldTimeStep());
    world.setCurrentTime(0.0);

    ConstraintForceSolver& cfs = world.constraintForceSolver;

    cfs.setGaussSeidelErrorCriterion(errorCriterion.value());
    cfs.setGaussSeidelMaxNumIterations(maxNumIterations);
    cfs.setContactDepthCorrection(
        contactCorrectionDepth.value(), contactCorrectionVelocityRatio.value());

    self->addPreDynamicsFunction(std::bind(&AISTSimulatorItemImpl::clearExternalForces, this));

    world.clearBodies();
    bodyIndexMap.clear();

    for(size_t i=0; i < simBodies.size(); ++i){
        addBody(static_cast<AISTSimBody*>(simBodies[i]));
    }

    cfs.setFriction(staticFriction, dynamicFriction);
    cfs.setContactCullingDistance(contactCullingDistance.value());
    cfs.setContactCullingDepth(contactCullingDepth.value());
    cfs.setCoefficientOfRestitution(epsilon);
    cfs.setCollisionDetector(self->collisionDetector());
    cfs.setSelfCollisionEnabled(self->isSelfCollisionEnabled());

    if(is2Dmode){
        cfs.set2Dmode(true);
    }

    world.initialize();

    ContactAttributeMap::iterator iter = contactAttributeMap.begin();
    while(iter != contactAttributeMap.end()){
        bool actualLinksFound = false;
        const IdPair<Link*>& linkPair = iter->first;
        LinkMap::iterator p0 = orgLinkToInternalLinkMap.find(linkPair(0));
        if(p0 != orgLinkToInternalLinkMap.end()){
            LinkMap::iterator p1 = orgLinkToInternalLinkMap.find(linkPair(1));
            if(p1 != orgLinkToInternalLinkMap.end()){

                Link* iLink0 = p0->second;
                Link* iLink1 = p1->second;
                actualLinksFound = true;

                const ContactAttribute& attr = iter->second;
                if(attr.staticFriction || attr.dynamicFriction){
                    cfs.setFriction(
                        iLink0, iLink1,
                        attr.staticFriction ? *attr.staticFriction : staticFriction,
                        attr.dynamicFriction ? *attr.dynamicFriction : dynamicFriction);
                }
                if(attr.epsilon != epsilon){
                    cfs.setCoefficientOfRestitution(iLink0, iLink1, attr.epsilon);
                }
                if(attr.collisionHandlerId){
                    cfs.setCollisionHandler(iLink0, iLink1, *attr.collisionHandlerId);
                }
            }
        }
        if(actualLinksFound){
            ++iter;
        } else {
            // remove the attribute for a non-existent link
            ContactAttributeMap::iterator current = iter++;
            contactAttributeMap.erase(current); 
        }
    }
    
    return true;
}


void AISTSimulatorItemImpl::addBody(AISTSimBody* simBody)
{
    DyBody* body = static_cast<DyBody*>(simBody->body());

    DyLink* rootLink = body->rootLink();
    rootLink->v().setZero();
    rootLink->dv().setZero();
    rootLink->w().setZero();
    rootLink->dw().setZero();
    rootLink->vo().setZero();
    rootLink->dvo().setZero();

    for(int i=0; i < body->numLinks(); ++i){
        Link* link = body->link(i);
        link->u() = 0.0;
        link->dq() = 0.0;
        link->ddq() = 0.0;
    }
    
    body->clearExternalForces();
    body->calcForwardKinematics(true, true);

    int bodyIndex;
    if(dynamicsMode.is(AISTSimulatorItem::HG_DYNAMICS)){
        ForwardDynamicsCBMPtr cbm = make_shared_aligned<ForwardDynamicsCBM>(body);
        cbm->setHighGainModeForAllJoints();
        bodyIndex = world.addBody(body, cbm);
    } else {
        bodyIndex = world.addBody(body);
    }
    bodyIndexMap[body] = bodyIndex;
}


void AISTSimulatorItemImpl::clearExternalForces()
{
    world.constraintForceSolver.clearExternalForces();
}


bool AISTSimulatorItem::stepSimulation(const std::vector<SimulationBody*>& activeSimBodies)
{
    if(!impl->dynamicsMode.is(KINEMATICS)){
         impl->world.calcNextState();
        return true;
    }

    // Kinematics mode
    if(!impl->isKinematicWalkingEnabled){
        for(size_t i=0; i < activeSimBodies.size(); ++i){
            activeSimBodies[i]->body()->calcForwardKinematics(true, true);
        }
    } else {
        for(size_t i=0; i < activeSimBodies.size(); ++i){
            SimulationBody* simBody = activeSimBodies[i];
            KinematicWalkBody* walkBody = dynamic_cast<KinematicWalkBody*>(simBody);
            if(!walkBody){
                simBody->body()->calcForwardKinematics(true, true);
            } else {
                walkBody->traverse.calcForwardKinematics(true, true);
                
                LeggedBodyHelper* legged = walkBody->legged;
                const int supportFootIndex = walkBody->supportFootIndex;
                int nextSupportFootIndex = supportFootIndex;
                Link* supportFoot = legged->footLink(supportFootIndex);
                Link* nextSupportFoot = supportFoot;
                const int n = legged->numFeet();
                for(int i=0; i < n; ++i){
                    if(i != supportFootIndex){
                        Link* foot = legged->footLink(i);
                        if(foot->p().z() < nextSupportFoot->p().z()){
                            nextSupportFootIndex = i;
                            nextSupportFoot = foot;
                        }
                    }
                }
                if(nextSupportFoot != supportFoot){
                    nextSupportFoot->p().z() = supportFoot->p().z();
                    walkBody->supportFootIndex = nextSupportFootIndex;
                    supportFoot = nextSupportFoot;
                    walkBody->traverse.find(supportFoot, true, true);
                    walkBody->traverse.calcForwardKinematics(true, true);
                }
            }
        }
    }
    return true;
}


void AISTSimulatorItem::finalizeSimulation()
{
    if(ENABLE_DEBUG_OUTPUT){
        impl->os.close();
    }
}


CollisionLinkPairListPtr AISTSimulatorItem::getCollisions()
{
    return impl->world.constraintForceSolver.getCollisions();
}


void AISTSimulatorItem::setForcedPosition(BodyItem* bodyItem, const Position& T)
{
    impl->setForcedPosition(bodyItem, T);
}


void AISTSimulatorItemImpl::setForcedPosition(BodyItem* bodyItem, const Position& T)
{
    if(SimulationBody* simBody = self->findSimulationBody(bodyItem)){
        {
            std::lock_guard<std::mutex> lock(forcedBodyPositionMutex);
            forcedPositionBody = static_cast<DyBody*>(simBody->body());
            forcedBodyPosition = T;
        }
        if(!forcedBodyPositionFunctionId){
            forcedBodyPositionFunctionId =
                self->addPostDynamicsFunction(
                    std::bind(&AISTSimulatorItemImpl::doSetForcedPosition, this));
        }
    }
}


bool AISTSimulatorItem::isForcedPositionActiveFor(BodyItem* bodyItem) const
{
    bool isActive = false;
    if(impl->forcedBodyPositionFunctionId){
        SimulationBody* simBody = const_cast<AISTSimulatorItem*>(this)->findSimulationBody(bodyItem);
        {
            std::lock_guard<std::mutex> lock(impl->forcedBodyPositionMutex);
            if(impl->forcedPositionBody == static_cast<DyBody*>(simBody->body())){
                isActive = true;
            }
        }
    }
    return isActive;
}


void AISTSimulatorItem::clearForcedPositions()
{
    if(impl->forcedBodyPositionFunctionId){
        removePostDynamicsFunction(*impl->forcedBodyPositionFunctionId);
        impl->forcedBodyPositionFunctionId = boost::none;
    }
}
    

void AISTSimulatorItemImpl::doSetForcedPosition()
{
    std::lock_guard<std::mutex> lock(forcedBodyPositionMutex);
    DyLink* rootLink = forcedPositionBody->rootLink();
    rootLink->setPosition(forcedBodyPosition);
    rootLink->v().setZero();
    rootLink->w().setZero();
    rootLink->vo().setZero();
    forcedPositionBody->calcSpatialForwardKinematics();
}


void AISTSimulatorItem::doPutProperties(PutPropertyFunction& putProperty)
{
    SimulatorItem::doPutProperties(putProperty);
    impl->doPutProperties(putProperty);
}


void AISTSimulatorItem::clearExtraJoint()
{
    impl->clearExtraJoint();
}


void AISTSimulatorItem::addExtraJoint(ExtraJoint& extrajoint)
{
    impl->addExtraJoint(extrajoint);
}


void AISTSimulatorItemImpl::clearExtraJoint()
{
    world.extrajoints.clear();
}


void AISTSimulatorItemImpl::addExtraJoint(ExtraJoint& extrajoint)
{
    world.extrajoints.push_back(extrajoint);
}


void AISTSimulatorItemImpl::doPutProperties(PutPropertyFunction& putProperty)
{
    putProperty(_("Dynamics mode"), dynamicsMode,
                std::bind(&Selection::selectIndex, &dynamicsMode, _1));
    putProperty(_("Integration mode"), integrationMode,
                std::bind(&Selection::selectIndex, &integrationMode, _1));
    putProperty(_("Gravity"), str(gravity), std::bind(toVector3, _1, std::ref(gravity)));
    putProperty.decimals(3).min(0.0);
    putProperty(_("Static friction"), staticFriction, changeProperty(staticFriction));
    putProperty(_("Slip friction"), dynamicFriction, changeProperty(dynamicFriction));
    putProperty(_("Contact culling distance"), contactCullingDistance,
                (std::bind(&FloatingNumberString::setNonNegativeValue, std::ref(contactCullingDistance), _1)));
    putProperty(_("Contact culling depth"), contactCullingDepth,
                (std::bind(&FloatingNumberString::setNonNegativeValue, std::ref(contactCullingDepth), _1)));
    putProperty(_("Error criterion"), errorCriterion,
                std::bind(&FloatingNumberString::setPositiveValue, std::ref(errorCriterion), _1));
    putProperty.min(1.0)(_("Max iterations"), maxNumIterations, changeProperty(maxNumIterations));
    putProperty(_("CC depth"), contactCorrectionDepth,
                std::bind(&FloatingNumberString::setNonNegativeValue, std::ref(contactCorrectionDepth), _1));
    putProperty(_("CC v-ratio"), contactCorrectionVelocityRatio,
                std::bind(&FloatingNumberString::setNonNegativeValue, std::ref(contactCorrectionVelocityRatio), _1));
    putProperty(_("Kinematic walking"), isKinematicWalkingEnabled,
                changeProperty(isKinematicWalkingEnabled));
    putProperty(_("2D mode"), is2Dmode, changeProperty(is2Dmode));
    putProperty(_("Old accel sensor mode"), isOldAccelSensorMode, changeProperty(isOldAccelSensorMode));
}


bool AISTSimulatorItem::store(Archive& archive)
{
    SimulatorItem::store(archive);
    return impl->store(archive);
}


bool AISTSimulatorItemImpl::store(Archive& archive)
{
    archive.write("dynamicsMode", dynamicsMode.selectedSymbol(), DOUBLE_QUOTED);
    archive.write("integrationMode", integrationMode.selectedSymbol(), DOUBLE_QUOTED);
    write(archive, "gravity", gravity);
    archive.write("staticFriction", staticFriction);
    archive.write("dynamicFriction", dynamicFriction);
    archive.write("cullingThresh", contactCullingDistance);
    archive.write("contactCullingDepth", contactCullingDepth);
    archive.write("errorCriterion", errorCriterion);
    archive.write("maxNumIterations", maxNumIterations);
    archive.write("contactCorrectionDepth", contactCorrectionDepth);
    archive.write("contactCorrectionVelocityRatio", contactCorrectionVelocityRatio);
    archive.write("kinematicWalking", isKinematicWalkingEnabled);
    archive.write("2Dmode", is2Dmode);
    archive.write("oldAccelSensorMode", isOldAccelSensorMode);
    return true;
}


bool AISTSimulatorItem::restore(const Archive& archive)
{
    SimulatorItem::restore(archive);
    return impl->restore(archive);
}


bool AISTSimulatorItemImpl::restore(const Archive& archive)
{
    string symbol;
    if(archive.read("dynamicsMode", symbol)){
        dynamicsMode.select(symbol);
    }
    if(archive.read("integrationMode", symbol)){
        integrationMode.select(symbol);
    }
    read(archive, "gravity", gravity);
    archive.read("staticFriction", staticFriction);
    if(!archive.read("dynamicFriction", dynamicFriction)){
        archive.read("slipFriction", dynamicFriction);
    }
    contactCullingDistance = archive.get("cullingThresh", contactCullingDistance.string());
    contactCullingDepth = archive.get("contactCullingDepth", contactCullingDepth.string());
    errorCriterion = archive.get("errorCriterion", errorCriterion.string());
    archive.read("maxNumIterations", maxNumIterations);
    contactCorrectionDepth = archive.get("contactCorrectionDepth", contactCorrectionDepth.string());
    contactCorrectionVelocityRatio = archive.get("contactCorrectionVelocityRatio", contactCorrectionVelocityRatio.string());
    archive.read("kinematicWalking", isKinematicWalkingEnabled);
    archive.read("2Dmode", is2Dmode);
    archive.read("oldAccelSensorMode", isOldAccelSensorMode);
    return true;
}

#ifdef ENABLE_SIMULATION_PROFILING
void AISTSimulatorItem::getProfilingNames(vector<string>& profilingNames)
{
    profilingNames.push_back("Collision detection time");
    profilingNames.push_back("Constraint force calculation time");
    profilingNames.push_back("Forward dynamics calculation time");
    profilingNames.push_back("Customizer calculation time");
}


void AISTSimulatorItem::getProfilingTimes(vector<double>& profilingToimes)
{
    double collisionTime = impl->world.constraintForceSolver.getCollisionTime();
    profilingToimes.push_back(collisionTime);
    profilingToimes.push_back(impl->world.forceSolveTime - collisionTime);
    profilingToimes.push_back(impl->world.forwardDynamicsTime);
    profilingToimes.push_back(impl->world.customizerTime);
}
#endif
