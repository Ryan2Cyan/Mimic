#pragma once
#include <utility/Logger.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>
#include <iomanip>

// Macros:
#define MIMIC_PROFILE_BEGIN_SESSION(name, filepath) ::MimicUtil::Instrumentor::Get().BeginSession(name, filepath)
#define MIMIC_PROFILE_END_SESSION() ::MimicUtil::Instrumentor::Get().EndSession()
#define MIMIC_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::MimicUtil::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
																			   ::MimicUtil::InstrumentationTimer timer##line(fixedName##line.Data)
#define MIMIC_PROFILE_SCOPE_LINE(name, line) MIMIC_PROFILE_SCOPE_LINE2(name, line)
#define MIMIC_PROFILE_SCOPE(name) MIMIC_PROFILE_SCOPE_LINE(name, __LINE__)
#define MIMIC_PROFILE_FUNCTION() MIMIC_PROFILE_SCOPE(__FUNCSIG__)


// Source: https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Debug/Instrumentor.h
namespace MimicUtil
{

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(m_Mutex); // Takes ownership of the mutex until out of scope.
			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				MIMIC_LOG_ERROR("Instrumentor::BeginSession('%') when session '%' already open.", name, m_CurrentSession->Name);
			}

			// Attempt to open file from given filepath, if able to open begin writing, otherwise error.
			m_OutputStream.open(filepath);
			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else MIMIC_LOG_ERROR("Instrumentor could not open results file '%'.", filepath);
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex); // Takes ownership of the mutex when writing to output.
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor() : m_CurrentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			// Instrumentor is scope-based, it will end on destruction (when it goes out of scope).
			EndSession();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before calling InternalEndSession().
		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			// Start the timer on construction (scope-based):
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			// Stop the timer on destruction (scope-based):
			if (!m_Stopped) Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			// Get the static instrumentor instance and write name & time:
			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils
	{

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex]) matchIndex++;
				if (matchIndex == K - 1) srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}


//namespace MimicUtil
//{
	//// #############################################################################
	//// timer stuct:
	//// #############################################################################
	//// Source: https://www.youtube.com/watch?v=YbYV8rRo9_A
	//template<typename Func>
	//struct Timer
	//{
	//	Timer(const char* name, Func&& lambdaFunc) : _name(name), _stopped(false), _func(lambdaFunc)
	//	{
	//		_startTimePoint = std::chrono::high_resolution_clock::now();
	//	};

	//	~Timer() // scope based: stop is called int he destructor
	//	{
	//		if (!_stopped) Stop();
	//	}

	//	void Stop()
	//	{
	//		const auto endTimePoint = std::chrono::high_resolution_clock::now();
	//		const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimePoint).time_since_epoch().count();
	//		const auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

	//		const float duration = end - start;
	//		const float milliSeconds = duration * 0.001;

	//		_stopped = true;

	//		_func({ _name, milliSeconds });
	//	}

	//private:
	//	std::chrono::time_point<std::chrono::high_resolution_clock> _startTimePoint;
	//	Func _func;
	//	const char* _name;
	//	bool _stopped = true;
	//};

	//// #############################################################################
	//// profile result stuct:
	//// #############################################################################
	//struct ProfileResult
	//{
	//	const char* Name;
	//	float Time;
	//};

	//std::vector<ProfileResult> ProfilerResults;

	//// #############################################################################
	//// performance counter stuct:
	//// #############################################################################
	//struct PerformanceCounter
	//{
	//	explicit PerformanceCounter();

	//	static std::shared_ptr<PerformanceCounter> Initialise();
	//	void StartPerformanceCounter() noexcept;
	//	const long long EndPerformanceCounter() noexcept;
	//	const long GetFPS() const noexcept;

	//private:
	//	long long _startTime;
	//	long long _elapsedTime;
	//};
// }