// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include <chrono>
#include <functional>
#include <mutex>
#include <unordered_map>
#include "xsapi/xbox_service_call_routed_event_args.h"

NAMESPACE_MICROSOFT_XBOX_SERVICES_CPP_BEGIN

#define DEFAULT_HTTP_TIMEOUT_SECONDS (30)
#if UNIT_TEST_SERVICES
    #define MIN_HTTP_TIMEOUT_SECONDS (0) // speed up unit tests
#else
    #define MIN_HTTP_TIMEOUT_SECONDS (5)
#endif
#define MIN_HTTP_TIMEOUT_MILLISECONDS (MIN_HTTP_TIMEOUT_SECONDS * 1000)
#define DEFAULT_LONG_HTTP_TIMEOUT_SECONDS (5 * 60)
#define DEFAULT_WEBSOCKET_TIMEOUT_SECONDS (60)
#define MAXIMUM_WEBSOCKETS_ACTIVATIONS_ALLOWED_PER_USER (5)
#define DEFAULT_HTTP_RETRY_WINDOW_SECONDS (20)
#define DEFAULT_RETRY_DELAY_SECONDS (2)
#define MIN_RETRY_DELAY_SECONDS (2)

/// <summary>
/// Enumeration values that indicate the trace levels of debug output for service diagnostics.
///
/// Setting the debug trace level to error or higher reports the last HRESULT, the current
/// function, the source file, and the line number for many trace points in the Xbox live code.
/// </summary>
enum class xbox_services_diagnostics_trace_level
{
    /// <summary>
    /// Output no tracing and debugging messages.
    /// </summary>
    off,

    /// <summary>
    /// Output error-handling messages.
    /// </summary>
    error,

    /// <summary>
    /// Output warnings and error-handling messages.
    /// </summary>
    warning,

    /// <summary>
    /// Output informational messages, warnings, and error-handling messages.
    /// </summary>
    info,

    /// <summary>
    /// Output all debugging and tracing messages.
    /// </summary>
    verbose
};

/// <summary>
/// Enum used with disable_asserts_for_xbox_live_throttling_in_dev_sandboxes()
/// </summary>
enum class xbox_live_context_throttle_setting
{
    /// <summary>
    /// Passed to xboxLiveContext->settings()->disable_asserts_for_xbox_live_throttling_in_dev_sandboxes()
    /// to warn code reviewers that there's an outstanding Xbox Live calling pattern issue that needs to be addressed.
    /// </summary>
    this_code_needs_to_be_changed_to_avoid_throttling
};

/// <summary>
/// Enum used with disable_asserts_ APIs
/// </summary>
enum class xbox_live_context_recommended_setting
{
    /// <summary>
    /// Passed to xboxLiveContext->settings()->disable_asserts_ APIs
    /// to warn code reviewers that there's an outstanding calling pattern issue that needs to be addressed.
    /// </summary>
    this_code_needs_to_be_changed_to_follow_best_practices
};

/// <summary>
/// Represents settings for an HTTP call.
/// </summary>
class xbox_live_context_settings
{
public:
    /// <summary>
    /// Creates a new xbox_live_context_settings object.
    /// </summary>
    _XSAPIIMP xbox_live_context_settings();

    /// <summary>
    /// Registers for all service call notifications.  Event handlers will receive xbox::services::xbox_service_call_routed_event_args
    /// </summary>
    /// <param name="handler">The event handler function to call.</param>
    /// <returns>
    /// A function_context object that can be used to unregister the event handler.
    /// </returns>
    _XSAPIIMP function_context add_service_call_routed_handler(_In_ std::function<void(const xbox::services::xbox_service_call_routed_event_args&)> handler);

    /// <summary>
    /// Unregisters from all service call notifications.
    /// </summary>
    /// <param name="context">The function_context object that was returned when the event handler was registered. </param>
    _XSAPIIMP void remove_service_call_routed_handler(_In_ function_context context);

    /// <summary>
    /// Indicates if events should be generated for service_call_routed_handler.
    /// </summary>
    _XSAPIIMP bool enable_service_call_routed_events() const;

    /// <summary>
    /// Set if events should be generated for service_call_routed_handler.
    /// </summary>
    _XSAPIIMP void set_enable_service_call_routed_events(_In_ bool value);

    /// <summary>
    /// DEPRECATED. Instead call xbox_live_services_settings::get_singleton_instance()->diagnostics_trace_level()
    /// Indicates the level of debug messages to send to the debugger's Output window.
    /// </summary>
    _XSAPIIMP_DEPRECATED xbox_services_diagnostics_trace_level diagnostics_trace_level() const;

    /// <summary>
    /// DEPRECATED. Instead call xbox_live_services_settings::get_singleton_instance()->set_diagnostics_trace_level(value)
    /// Sets the level of debug messages to send to the debugger's Output window.
    /// </summary>
    _XSAPIIMP_DEPRECATED void set_diagnostics_trace_level(_In_ xbox_services_diagnostics_trace_level value);

    /// <summary>
    /// DEPRECATED.  This value is ignored and is now automatically calculated to ensure the http_timeout_window() is obeyed
    /// This means that calls will be more deterministic about the time window they return in.
    /// </summary>
    _XSAPIIMP const std::chrono::seconds& http_timeout() const;

    /// <summary>
    /// DEPRECATED.  This value is ignored and is now automatically calculated to ensure the http_timeout_window() is obeyed
    /// This means that calls will be more deterministic about the time window they return in.
    /// </summary>
    _XSAPIIMP void set_http_timeout(_In_ std::chrono::seconds value);

    /// <summary>
    /// Gets the connect, send, and receive timeouts for HTTP socket operations of long calls such as Title Storage calls.
    /// Default is 5 minutes.  Calls that take longer than this are aborted.    
    /// </summary>
    _XSAPIIMP const std::chrono::seconds& long_http_timeout() const;

    /// <summary>
    /// Sets the connect, send, and receive timeouts for HTTP socket operations of long calls such as Title Storage calls.
    /// Default is 5 minutes.  Calls that take longer than this are aborted.
    /// Take care when setting this to smaller values as some calls like Title Storage may take a few minutes to complete.
    /// </summary>
    _XSAPIIMP void set_long_http_timeout(_In_ std::chrono::seconds value);

    /// <summary>
    /// Gets the HTTP retry delay in seconds.
    ///
    /// Retries are delayed using a exponential back off.  By default, it will delay 2 seconds then the 
    /// next retry will delay 4 seconds, then 8 seconds, and so on up to a max of 1 min until either
    /// the call succeeds or the http_timeout_window is reached, at which point the call will fail.
    /// The delay is also jittered between the current and next delay to spread out service load.
    /// The default for http_timeout_window is 20 seconds and can be changed using set_http_timeout_window()
    /// 
    /// If the service returns an an HTTP error with a "Retry-After" header, then all future calls to that API 
    /// will immediately fail with the original error without contacting the service until the "Retry-After" 
    /// time has been reached.
    ///
    /// Idempotent service calls are retried when a network error occurs or the server responds with one of these HTTP status codes:
    /// 408 (Request Timeout)
    /// 429 (Too Many Requests)
    /// 500 (Internal Server Error)
    /// 502 (Bad Gateway)
    /// 503 (Service Unavailable)
    /// 504 (Gateway Timeout)
    /// </summary>
    _XSAPIIMP const std::chrono::seconds& http_retry_delay() const;

    /// <summary>
    /// Sets the HTTP retry delay in seconds. The default and minimum delay is 2 seconds.
    /// 
    /// Retries are delayed using a exponential back off.  By default, it will delay 2 seconds then the 
    /// next retry will delay 4 seconds, then 8 seconds, and so on up to a max of 1 min until either
    /// the call succeeds or the http_timeout_window is reached, at which point the call will fail.
    /// The delay is also jittered between the current and next delay to spread out service load.
    /// The default for http_timeout_window is 20 seconds and can be changed using set_http_timeout_window()
    /// 
    /// If the service returns an an HTTP error with a "Retry-After" header, then all future calls to that API 
    /// will immediately fail with the original error without contacting the service until the "Retry-After" 
    /// time has been reached.
    ///
    /// Idempotent service calls are retried when a network error occurs or the server responds with one of these HTTP status codes:
    /// 408 (Request Timeout)
    /// 429 (Too Many Requests)
    /// 500 (Internal Server Error)
    /// 502 (Bad Gateway)
    /// 503 (Service Unavailable)
    /// 504 (Gateway Timeout)
    /// </summary>
    _XSAPIIMP void set_http_retry_delay(_In_ std::chrono::seconds value);

    /// <summary>
    /// Gets the HTTP timeout window in seconds.
    ///
    /// This controls how long to spend attempting to retry idempotent service calls before failing.
    /// The default is 20 seconds
    ///
    /// Idempotent service calls are retried when a network error occurs or the server responds with one of these HTTP status codes:
    /// 408 (Request Timeout)
    /// 429 (Too Many Requests)
    /// 500 (Internal Server Error)
    /// 502 (Bad Gateway)
    /// 503 (Service Unavailable)
    /// 504 (Gateway Timeout)
    /// </summary>
    _XSAPIIMP const std::chrono::seconds& http_timeout_window() const;

    /// <summary>
    /// Sets the HTTP timeout window in seconds.
    ///
    /// This controls how long to spend attempting to retry idempotent service calls before failing.
    /// The default is 20 seconds.  Set to 0 to turn off retry.
    ///
    /// Idempotent service calls are retried when a network error occurs or the server responds with one of these HTTP status codes:
    /// 408 (Request Timeout)
    /// 429 (Too Many Requests)
    /// 500 (Internal Server Error)
    /// 502 (Bad Gateway)
    /// 503 (Service Unavailable)
    /// 504 (Gateway Timeout)
    /// </summary>
    _XSAPIIMP void set_http_timeout_window(_In_ std::chrono::seconds value);

    /// <summary>
    /// Gets the web socket timeout window in seconds.
    /// </summary>
    _XSAPIIMP const std::chrono::seconds& websocket_timeout_window() const;

    /// <summary>
    /// Sets the web socket timeout window in seconds.
    /// Controls how long to spend attempting to retry establishing a websocket connection before failing. 
    /// Default is 300 seconds.  Set to 0 to turn off retry.
    /// </summary>
    _XSAPIIMP void set_websocket_timeout_window(_In_ std::chrono::seconds value);

    /// <summary>
    /// Gets whether to use the dispatcher for event routing
    /// </summary>
    _XSAPIIMP bool use_core_dispatcher_for_event_routing() const;

    /// <summary>
    /// Controls whether to use the CoreDispatcher from the User object to route events through. 
    /// This is required to be true if using events with JavaScript.
    /// </summary>
    _XSAPIIMP void set_use_core_dispatcher_for_event_routing(_In_ bool value);

    /// <summary>
    /// Disables asserts for Xbox Live throttling in dev sandboxes.
    /// The asserts will not fire in RETAIL sandbox, and this setting has has no affect in RETAIL sandboxes.
    /// It is best practice to not call this API, and instead adjust the calling pattern but this is provided
    /// as a temporary way to get unblocked while in early stages of game development.
    /// </summary>
    _XSAPIIMP void disable_asserts_for_xbox_live_throttling_in_dev_sandboxes(
        _In_ xbox_live_context_throttle_setting setting
        );

    /// <summary>
    /// Disables asserts for having maximum number of websockets being activated 
    /// i.e. MAXIMUM_WEBSOCKETS_ACTIVATIONS_ALLOWED_PER_USER (5) per user per title instance.
    /// It is best practice to not call this API, and instead adjust the calling pattern but this is provided
    /// as a temporary way to get unblocked while in early stages of game development.
    /// </summary>
    _XSAPIIMP void disable_asserts_for_maximum_number_of_websockets_activated(
        _In_ xbox_live_context_recommended_setting setting
        );

    /// <summary>
    /// Gets whether to use the xplatqos server for qos calls.
    /// </summary>
    _XSAPIIMP bool use_crossplatform_qos_servers() const;

    /// <summary>
    /// Controls whether we use cross platform qos endpoints or not.
    /// In some case if you are shipping with TV_API enabled, you want to be able to choose.
    /// </summary>
    _XSAPIIMP void set_use_crossplatform_qos_servers(_In_ bool value);

public:
    // Internal public function
#if UWP_API || UNIT_TEST_SERVICES
    /// <summary>
    /// Sets the CoreDispatcher 
    /// </summary>
    static void _Set_dispatcher(_In_opt_ Platform::Object^ coreDispatcherObj);
#endif
#if TV_API || UWP_API || UNIT_TEST_SERVICES
    static Windows::UI::Core::CoreDispatcher^ _s_dispatcher;
#endif

    void _Raise_service_call_routed_event(_In_ const xbox::services::xbox_service_call_routed_event_args& result);

private:

    bool m_enableServiceCallRoutedEvents;
    std::chrono::seconds m_httpTimeout;
    std::chrono::seconds m_longHttpTimeout;
    std::chrono::seconds m_httpRetryDelay;
    std::chrono::seconds m_httpTimeoutWindow;

    std::chrono::seconds m_websocketTimeoutWindow;
    bool m_useCoreDispatcherForEventRouting;
    
    bool m_useXplatQosServer;
};


NAMESPACE_MICROSOFT_XBOX_SERVICES_CPP_END