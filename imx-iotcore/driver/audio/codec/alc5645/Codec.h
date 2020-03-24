/* Copyright (c) Microsoft Corporation. All rights reserved.
   Licensed under the MIT License.

Module Name:

    Codec.h

Environment:

    User-mode Driver Framework 2

*/
#pragma once


NTSTATUS
CodecInitialize(
    _In_ PDEVICE_CONTEXT DeviceContext
    );

NTSTATUS
CodecInitializeHeadphoneOutJack(
    _In_ PDEVICE_CONTEXT DeviceContext
    );