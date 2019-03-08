//
//  Dispatch.asio.hpp
//  Copyright (c) 2014-2019, MyMonero.com
//
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modification, are
//  permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//	conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//	of conditions and the following disclaimer in the documentation and/or other
//	materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its contributors may be
//	used to endorse or promote products derived from this software without specific
//	prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
//  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
//  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef Dispatch_asio_hpp
#define Dispatch_asio_hpp

#include <string>
#include "./Dispatch_Interface.hpp"
#include <boost/asio.hpp>

namespace Dispatch
{
	using namespace std;
	using namespace boost::asio;
	//
	struct Timers_asio: public Timers
	{
		Timers_asio(io_context& ctx):
			_ctx(ctx)
		{
		}
		~Timers_asio() {}
		void schedule_timer(uint32_t ms, std::function<void()>&& fn)
		{
			auto t = new steady_timer(_ctx, boost::asio::chrono::milliseconds(ms));
			t->async_wait([fn = std::move(fn), t](const boost::system::error_code &)
			{
				fn();
				delete t;
			});
		}
	private:
		io_context &_ctx;
		executor_work_guard<io_context::executor_type> _guard = make_work_guard(_ctx);
	};
	//
	struct io_ctx_thread_holder
	{
		io_ctx_thread_holder(io_context& ctx):
			_ctx(ctx),
			_thread([this]() { _ctx.run(); })
		{
		}
		~io_ctx_thread_holder()
		{
			_thread.join();
		}
	private:
		io_context &_ctx;
		std::thread _thread;
	};
}

#endif /* Dispatch_asio_hpp */
