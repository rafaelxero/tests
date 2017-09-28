#include <boost/make_shared.hpp>
#include <iostream>
#include <memory>
#include <Eigen/Core>

struct A
{
  A() : a(0), b(0) {}
  A(int a) : a(a), b(a) {}
  A(int a, double b) : a(a), b(b) {}
  int a;
  double b;
};

//template<class T, typename ... Args>
//std::shared_ptr<T> make_shared_aligned(Args&&... args)
//{
//  return std::allocate_shared<T>(Eigen::aligned_allocator<T>(), std::forward<Args>(args)...);
//}
//
//template<>
template<class T>
std::shared_ptr<T> make_shared_aligned()
{
  auto p = boost::allocate_shared<T>(Eigen::aligned_allocator<T>());
  return std::shared_ptr<T>(p.get(), [p](...) mutable { p.reset(); });
}

int main()
{
  std::shared_ptr<A> a = make_shared_aligned<A>();
  std::shared_ptr<int> b = std::allocate_shared<int>(Eigen::aligned_allocator<int>());
  std::cout << a->a << " " << a->b << std::endl;
  return 0;
}
