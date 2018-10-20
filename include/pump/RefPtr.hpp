//
// Created by finefenng on 2018/10/9.
//

#ifndef PUMP_REFPTR_H
#define PUMP_REFPTR_H

#include <type_traits>

namespace pump
{

template <typename T>
class RefPtr
{
  template <typename W> friend class RefPtr;
  typedef T* POINT_TYPE;
  typedef RefPtr<T> THIS_TYPE;
  typename std::remove_cv<T>::type* p_;
 public:
  typedef T ELEMENT_TYPE;

  constexpr RefPtr() noexcept
  : p_(nullptr)
  {}

  constexpr RefPtr(std::nullptr_t) noexcept
  : p_(nullptr)
  {}

  ~RefPtr()
  {
    if (p_ != nullptr) {
      p_->_ref_drop();
    }
  }

  inline RefPtr(THIS_TYPE&& r) noexcept
  : p_(r.p_)
  {
    r.p_ = nullptr;
  }










};


}






#endif //PUMP_REFPTR_H
