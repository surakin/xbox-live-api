// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

NAMESPACE_MICROSOFT_XBOX_SERVICES_CPP_BEGIN

class xbox_live_context;
/// <summary>
/// Contains classes and enumerations that let you retrieve
/// information about a player's Xbox Live profile.
/// </summary>
namespace social {

    class profile_service_impl;
    class xbox_user_profile_internal;

    /// <summary> 
    /// Represents a user's Xbox Live profile.
    /// </summary>
    class xbox_user_profile
    {
    public:
        /// <summary>
        /// Internal function
        /// </summary>
        _XSAPIIMP xbox_user_profile();

        /// <summary>
        /// Internal function
        /// </summary>
        _XSAPIIMP xbox_user_profile(
            _In_ std::shared_ptr<xbox_user_profile_internal> internalObj
            );

        /// <summary>
        /// The user's display name to be used in application UI.  This value is privacy gated and could
        /// be a user's real name or their Gamertag.
        /// </summary>
        _XSAPIIMP string_t app_display_name() const;

        /// <summary>
        /// Uri for the user's display picture to be used in application UI.
        /// The Uri is a resizable Uri. It can be used to specify one of the following sizes and formats by appending &apos;&amp;format={format}&amp;w={width}&amp;h={height}:
        /// Format: png
        /// Width   Height
        /// 64      64
        /// 208     208
        /// 424     424
        /// </summary>
        _XSAPIIMP const web::uri& app_display_picture_resize_uri() const;

        /// <summary>
        /// The user's display name to be used in game UI.  This value is privacy gated and could
        /// be a user's real name or their Gamertag.
        /// </summary>
        _XSAPIIMP string_t game_display_name() const;

        /// <summary>
        /// Uri for the user's display picture to be used in games.
        /// The Uri is a resizable Uri. It can be used to specify one of the following sizes and formats by appending &apos;&amp;format={format}&amp;w={width}&amp;h={height}:
        /// Format: png
        /// Width   Height
        /// 64      64
        /// 208     208
        /// 424     424
        /// </summary>
        _XSAPIIMP const web::uri& game_display_picture_resize_uri() const;

        /// <summary>
        /// The user's Gamerscore.
        /// </summary>
        _XSAPIIMP string_t gamerscore() const;

        /// <summary>
        /// The user's Gamertag.
        /// </summary>
        _XSAPIIMP string_t gamertag() const;

        /// <summary>
        /// The user's Xbox user ID.
        /// </summary>
        _XSAPIIMP string_t xbox_user_id() const;

    private:
        std::shared_ptr<xbox_user_profile_internal> m_internalObj;
    };

    /// <summary>
    /// Services that manage user profile.
    /// </summary>
    class profile_service
    {
    public:
        /// <summary>
        /// Gets a user profile for a specific Xbox user.
        /// </summary>
        /// <param name="xboxUserId">The Xbox User ID of the user to get the profile for.</param>
        /// <returns>
        /// Returns a concurrency::task&lt;T&gt; object that represents the state of the asynchronous operation.
        /// The result of the asynchronous operation is an xbox_user_profile object.
        /// </returns>
        /// <remarks>Calls V2 GET /users/batch/profile/settings</remarks>
        _XSAPIIMP pplx::task<xbox::services::xbox_live_result<xbox_user_profile>> get_user_profile(
        _In_ string_t xboxUserId
        );

        /// <summary>
        /// Gets one or more user profiles for a collection of specified Xbox users.
        /// </summary>
        /// <param name="xboxUserIds">The collection of Xbox User IDs of the users to get profiles for</param>
        /// <returns>
        /// Returns a concurrency::task&lt;T&gt; object that represents the state of the asynchronous operation.
        /// The result of the asynchronous operation is a collection of xbox_user_profile objects.
        /// </returns>
        /// <remarks>Calls V2 GET /users/batch/profile/settings</remarks>
        _XSAPIIMP pplx::task<xbox::services::xbox_live_result<std::vector<xbox_user_profile>>> get_user_profiles(
        _In_ const std::vector<string_t>& xboxUserIds
        );

        /// <summary>
        /// Gets user profiles for users in a specified social group.
        /// </summary>
        /// <param name="socialGroup">The name of the social group of users to search.
        /// See xbox::services::social::social_group_constants for the latest options.</param>
        /// <returns>
        /// Returns a concurrency::task&lt;T&gt; object that represents the state of the asynchronous operation.
        /// The result of the asynchronous operation is a collection of xbox_user_profile objects.
        /// </returns>
        /// <remarks>Calls V2 GET /users/{userId}/profile/settings/people/{socialGroup}</remarks>
        _XSAPIIMP pplx::task<xbox::services::xbox_live_result< std::vector< xbox_user_profile>>> get_user_profiles_for_social_group(
        _In_ const string_t& socialGroup
        );

    private:
        profile_service() {};

        profile_service(_In_ std::shared_ptr<profile_service_impl> serviceImpl);

        std::shared_ptr<profile_service_impl> m_serviceImpl;

        friend xbox_live_context;
    };
}

NAMESPACE_MICROSOFT_XBOX_SERVICES_CPP_END
