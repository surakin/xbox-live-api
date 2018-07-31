// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#pragma once

#if TV_API
    NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_BEGIN
        ref class XboxLiveUser;
    NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_END
#elif UNIT_TEST_SERVICES || !XSAPI_CPP
    NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_BEGIN
        ref class XboxLiveUser;
    NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_END
#else
    NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_BEGIN
        class xbox_live_user;
    NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_END
#endif

NAMESPACE_MICROSOFT_XBOX_SERVICES_CPP_BEGIN

class xbox_live_context_settings;
class http_call_response_internal;

/// <summary>
/// Enumerates the type of structured data contained in the http response body.
/// </summary>
enum class http_call_response_body_type
{
    /// <summary>
    /// The response body consists of a string.
    /// </summary>
    string_body,

    /// <summary>
    /// The response body consists of a vector of bytes.
    /// </summary>
    vector_body,

    /// <summary>
    /// The response body consists of a JavaScript Object Notation (JSON) object.
    /// </summary>
    json_body
};

// Forward declare
enum class xbox_live_api;

/// <summary>
/// Represents an http response from the Xbox Live service.
/// </summary>
class http_call_response
{
public:
    /// <summary>
    /// Internal function
    /// </summary>
    http_call_response(
        _In_ std::shared_ptr<http_call_response_internal> internalObj
        );

#ifndef DEFAULT_MOVE_ENABLED
    http_call_response(http_call_response&& other);
    http_call_response& operator=(http_call_response&& other);
#endif

    /// <summary>
    /// Gets the body type of the response.
    /// </summary>
    _XSAPIIMP http_call_response_body_type body_type() const;

    /// <summary>
    /// Gets the response body of the response as a string.
    /// </summary>
    _XSAPIIMP string_t response_body_string() const;

    /// <summary>
    /// Gets the response body of the response as a JSON value.
    /// </summary>
    _XSAPIIMP const web::json::value& response_body_json() const;

    /// <summary>
    /// Gets the response body of the response as a byte vector.
    /// </summary>
    _XSAPIIMP std::vector<unsigned char> response_body_vector() const;

    /// <summary>
    /// Gets the http headers of the response.
    /// </summary>
    _XSAPIIMP web::http::http_headers response_headers() const;

    /// <summary>
    /// Gets the http status of the response.
    /// </summary>
    _XSAPIIMP uint32_t http_status() const;

    /// <summary>
    /// Gets the error code of the response.
    /// </summary>
    _XSAPIIMP const std::error_code& err_code() const;

    /// <summary>
    /// Gets the error message of the response.
    /// </summary>
    _XSAPIIMP std::string err_message() const;

    /// <summary>
    /// Gets the eTag of the response.
    /// </summary>
    _XSAPIIMP string_t e_tag() const;

    /// <summary>
    /// Gets the response date of the response.
    /// </summary>
    _XSAPIIMP string_t response_date() const;

    /// <summary>
    /// Gets the "retry after" value found in the response.
    /// </summary>
    _XSAPIIMP const std::chrono::seconds& retry_after() const;

    /// <summary>
    /// Internal function - TODO remove after migrating all APIs
    /// </summary>
    std::shared_ptr<http_call_response_internal> _Internal_response() const;

private:
    std::shared_ptr<http_call_response_internal> m_internalObj;
};

class http_call
{
public:
    /// <summary>
    /// Attach the Xbox Live token, sign the request, send the request to the service, and return the response.
    /// </summary>
    virtual pplx::task<std::shared_ptr<http_call_response>> get_response_with_auth(
        _In_ http_call_response_body_type httpCallResponseBodyType = http_call_response_body_type::json_body
        ) = 0;

#if XSAPI_XDK_AUTH
    virtual pplx::task<std::shared_ptr<http_call_response>> get_response_with_auth(
        _In_ Windows::Xbox::System::User^ user,
        _In_ http_call_response_body_type httpCallResponseBodyType = http_call_response_body_type::json_body,
        _In_ bool allUsersAuthRequired = false
        ) = 0;
#endif 

#if XSAPI_NONXDK_CPP_AUTH
    virtual pplx::task<std::shared_ptr<http_call_response>> get_response_with_auth(
        _In_ std::shared_ptr<system::xbox_live_user> user,
        _In_ http_call_response_body_type httpCallResponseBodyType = http_call_response_body_type::json_body,
        _In_ bool allUsersAuthRequired = false
        ) = 0;
#endif 

#if XSAPI_NONXDK_WINRT_AUTH
    virtual pplx::task<std::shared_ptr<http_call_response>> get_response_with_auth(
        _In_ Microsoft::Xbox::Services::System::XboxLiveUser^ user,
        _In_ http_call_response_body_type httpCallResponseBodyType = http_call_response_body_type::json_body,
        _In_ bool allUsersAuthRequired = false
        ) = 0;
#endif
    
    /// <summary>
    /// Send the request without authentication and get the response.
    /// </summary>
    virtual pplx::task< std::shared_ptr<http_call_response> > get_response(
        _In_ http_call_response_body_type httpCallResponseBodyType
        ) = 0;

    /// <summary>
    /// Sets the request body using a string.
    /// </summary>
    virtual void set_request_body(_In_ const string_t& value) = 0;

    /// <summary>
    /// Sets the request body using a JSON value.
    /// </summary>
    virtual void set_request_body(_In_ const web::json::value& value) = 0;

    /// <summary>
    /// Sets the request body using a byte array value.
    /// </summary>
    virtual void set_request_body(_In_ const std::vector<uint8_t>& value) = 0;

    /// <summary>
    /// Sets a custom header.
    /// </summary>
    virtual void set_custom_header(
        _In_ const string_t& headerName,
        _In_ const string_t& headerValue) = 0;

    /// <summary>
    /// Sets if this is a long http call, and should use the long_http_timeout setting.
    /// </summary>
    virtual void set_long_http_call(_In_ bool value) = 0;

    /// <summary>
    /// Gets if this is a long http call, and should use the long_http_timeout setting.
    /// </summary>
    virtual bool long_http_call() const = 0;

    /// <summary>
    /// Sets if retry is allowed during this call.
    /// </summary>
    virtual void set_retry_allowed(_In_ bool value) = 0;

    /// <summary>
    /// Get if retry is allowed during this call.
    /// </summary>
    virtual bool retry_allowed() const = 0;

    /// <summary>
    /// Sets the content type header value for this call.
    /// </summary>
    virtual void set_content_type_header_value(_In_ const string_t& value) = 0;

    /// <summary>
    /// Gets the content type header value for this call.
    /// </summary>
    virtual string_t content_type_header_value() const = 0;

    /// <summary>
    /// Sets the Xbox Live contract version header value for this call.
    /// </summary>
    virtual void set_xbox_contract_version_header_value(_In_ const string_t& value) = 0;

    /// <summary>
    /// Gets the Xbox Live contract version header value for this call.
    /// </summary>
    virtual string_t xbox_contract_version_header_value() const = 0;

    /// <summary>
    /// Gets the server name for this call.
    /// </summary>
    virtual string_t server_name() const = 0;

    /// <summary>
    /// Gets the path for this call.
    /// </summary>
    virtual const web::uri& path_query_fragment() const = 0;

    /// <summary>
    /// Gets the http method for this call.
    /// </summary>
    virtual string_t http_method() const = 0;

    /// <summary>
    /// Sets a flag indicating if default headers should be added or not.
    /// </summary>
    virtual void set_add_default_headers(_In_ bool value) = 0;
    
    /// <summary>
    /// Internal function
    /// </summary>
    virtual bool add_default_headers() const = 0;

    virtual pplx::task<std::shared_ptr<http_call_response>> get_response_with_auth(
        _In_ const std::shared_ptr<xbox::services::user_context>& userContext,
        _In_ http_call_response_body_type httpCallResponseBodyType = http_call_response_body_type::json_body,
        _In_ bool allUsersAuthRequired = false
        ) = 0;

    virtual ~http_call(){}
};

_XSAPIIMP std::shared_ptr<http_call> create_xbox_live_http_call(
    _In_ const std::shared_ptr<xbox_live_context_settings>& xboxLiveContextSettings,
    _In_ const string_t& httpMethod,
    _In_ const string_t& serverName,
    _In_ const web::uri& pathQueryFragment
    );

NAMESPACE_MICROSOFT_XBOX_SERVICES_CPP_END