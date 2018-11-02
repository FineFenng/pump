#ifndef PUMP_REFBASE_H
#define PUMP_REFBASE_H

#include <atomic>

#include <pump/Common.h>


namespace pump {

	class RefBase {
	PUMP_DECLARE_DELETE_COPYABLE(RefBase)
	protected:
		RefBase()
			: ref_count_(0) { }

		virtual ~RefBase() = default;

	private:

		void ref_grab() { 
			ref_count_.fetch_add(1, std::memory_order_acq_rel);
		}

		void ref_drop() {
			ref_count_.fetch_sub(1, std::memory_order_acq_rel);
		}

		long ref_count() const {
			return ref_count_.load(std::memory_order_acquire);
		}



	private:
		std::atomic<long> ref_count_;


	};

}
#endif
