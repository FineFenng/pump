//
// Created by finefenng on 2018/10/9.
//

#ifndef PUMP_REFPTR_H
#define PUMP_REFPTR_H

#include <type_traits>
#include <pump/Common.h>


namespace pump {

	template <typename T>
	class RefPtr {
		template <typename W>
		friend class RefPtr;
		typedef T* POINT_TYPE;
		typedef RefPtr<T> THIS_TYPE;
		typename std::remove_cv<T>::type* p_;
	public:
		typedef T ELEMENT_TYPE;

		// Constructor Function
		constexpr RefPtr() noexcept
			: p_(nullptr) {
		}

		// Constructor Function
		constexpr RefPtr(std::nullptr_t) noexcept
			: p_(nullptr) {
		}

		// Destroy Function
		~RefPtr() {
			if (p_ != nullptr) {
				p_->ref_drop();
			}
		}

		// Copy Constructor Function
		inline RefPtr(const THIS_TYPE& r) noexcept
			: p_(r.p_) {
			if (p_ != nullptr) {
				p_->ref_grab();
			}
		}

		// Move Constructor Function
		inline RefPtr(THIS_TYPE&& r) noexcept
			: p_(r.p_) {
			r.p_ = nullptr;
		}

		template <typename Form>
		RefPtr(const RefPtr<Form>& r, const ELEMENT_TYPE* p):
			p_(p) {
			if (p_ != nullptr) {
				p_->ref_grab();
				(void)r;
			}
		}

		template <typename TpRel,
		          class = typename std::enable_if<std::is_convertible<TpRel*, ELEMENT_TYPE*>::value>::type>
		RefPtr(const TpRel* p)
			: p_(p) {
			if (p_ != nullptr) {
				p->ref_grab();
			}
		}

		template <typename TpRel,
		          class = typename std::enable_if<std::is_convertible<TpRel*, ELEMENT_TYPE*>::value>::type>
		RefPtr(RefPtr<TpRel>&& r)
			: p_(r.p_) {
			r.p_ = nullptr;
		}

		RefPtr& operator=(const THIS_TYPE& r) {
			THIS_TYPE(r).swap(*this);
			return *this;
		}

		RefPtr& operator=(THIS_TYPE&& r) noexcept {
			if (p_ != nullptr) {
				p_->ref_drop();
			}
			p_ = r.p_;
			r.p_ = nullptr;
			return *this;
		}

		void swap(THIS_TYPE& other) noexcept {
			std::swap(p_, other.p_);
		}

		long ref_count() const noexcept {
			return p_ == nullptr ? 0 : p_->ref_count();
		}

		const T*& operator->() const {
			return p_;
		}

		POINT_TYPE& operator->() {
			return p_;
		}

		const T& operator*() const {
			return *p_;
		}

		T& operator*() {
			return *p_;
		}

		POINT_TYPE get() const {
			return p_;
		}

		bool operator ==(const THIS_TYPE& r) const { return p_ == r.p_; }

		bool operator !=(const THIS_TYPE& r) const { return p_ != r.p_; }

		bool operator >(const THIS_TYPE& r) const { return p_ > r.p_; }

		bool operator <(const THIS_TYPE& r) const { return p_ < r.p_; }

		bool operator ==(const POINT_TYPE& p) const { return p_ == p; }

		bool operator !=(const POINT_TYPE& p) const { return p_ != p; }

		bool operator >(const POINT_TYPE& p) const { return p_ > p; }

		bool operator <(const POINT_TYPE& p) const { return p_ < p; }


	};

	template <typename TTM, typename ...Args>
	inline RefPtr<TTM> MakeRef(Args&&... args) {
		TTM* t = new TTM(std::forward<Args>(args)...);
		return RefPtr<TTM>(t);
	}


}

#define PMRP pump::RefPtr

#endif //PUMP_REFPTR_H
