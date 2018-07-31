// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "pch.h"
#include <stdio.h>
#include "collection.h"


class Utils
{
public:
    static Platform::String^ Utils::FormatString( LPCWSTR strMsg, ... );

    static Platform::String^ ConvertHResultToErrorName( HRESULT hr );
    static Platform::String^ GetErrorString( HRESULT hr );

    static bool IsStringEqualCaseInsenstive( Platform::String^ xboxUserId1, Platform::String^ xboxUserId2 );
};