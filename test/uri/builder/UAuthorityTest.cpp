/*
 
 * Copyright (c) 2024 General Motors GTO LLC
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 * SPDX-FileType: SOURCE
 * SPDX-FileCopyrightText: 2023 General Motors GTO LLC
 * SPDX-License-Identifier: Apache-2.0
 
 
 */
#include <string>
//#include <iostream>
#include <cgreen/cgreen.h>
#include "uprotocol-cpp/uri/builder/BuildUAuthority.h"
#include "uprotocol-cpp/uri/serializer/MicroUriSerializer.h"


using namespace cgreen;
using namespace uprotocol::uri;

#define assertTrue(a) assert_true(a)
#define assertEquals(a, b) assert_true((b) == (a))
#define assertFalse(a) assert_false(a)

Describe(UAuthority);

BeforeEach(UAuthority) {
    // Dummy
}

AfterEach(UAuthority) {
    // Dummy
}

// Make sure the toString works.
static void testToString() {
    auto long_remote = BuildUAuthority().setName("VCU", "my_VIN").build();
    assertTrue(long_remote.remote_case() != uprotocol::v1::UAuthority::REMOTE_NOT_SET);
    assertFalse(long_remote.name().empty());
    assertEquals("vcu.my_vin", long_remote.name());

    std::string address = "127.0.0.1";
    auto micro_remote = BuildUAuthority().setIp(address).build();
    assertTrue(micro_remote.has_ip());

    auto local = BuildUAuthority().build();
    assertTrue(isEmpty(local));
    assertTrue(local.remote_case() == uprotocol::v1::UAuthority::REMOTE_NOT_SET);

    auto empty = BuildUAuthority().build();
    assertTrue(isEmpty(local));
}

// Make sure the toString works with case sensitivity.
static void testToStringCaseSensitivity() {
    auto u_authority = BuildUAuthority().setName("VCU", "my_VIN").build();
    assertFalse(isEmpty(u_authority));
    assertTrue(u_authority.has_name());
    assertEquals("vcu.my_vin", u_authority.name());
}

// Test create a empty uAuthority.
static void testEmptyUAuthority() {
    auto u_authority = BuildUAuthority().build();
    assertFalse(u_authority.has_ip());
    assertFalse(u_authority.has_name());
    assertFalse(u_authority.has_id());
    assertTrue(isLocal(u_authority));
    assertFalse(isRemote(u_authority));
    assertFalse(isResolved(u_authority));
    assertTrue(isEmpty(u_authority));
    assertTrue(isMicroForm(u_authority));
    assertTrue(isLongForm(u_authority));
}

// Test create a local uAuthority.
static void testLocalUAuthority() {
    auto u_authority = BuildUAuthority().build();
    assertFalse(u_authority.has_ip());
    assertTrue(isLocal(u_authority));
    assertFalse(isRemote(u_authority));
    assertFalse(isResolved(u_authority));
    assertTrue(isEmpty(u_authority));
    assertTrue(isMicroForm(u_authority));
    assertTrue(isLongForm(u_authority));
}

// Test create a remote uAuthority that supports long UUris.
static void testLongRemoteUAuthority() {
    std::string device = "vcu";
    std::string domain = "myvin";
    auto u_authority = BuildUAuthority().setName(device, domain).build();
    assertFalse(u_authority.has_ip());
    assertFalse(u_authority.has_id());
    assertTrue(u_authority.has_name());
    assertEquals("vcu.myvin", u_authority.name());
    assertFalse(isLocal(u_authority));
    assertTrue(isRemote(u_authority));
    assertFalse(isResolved(u_authority));
    assertFalse(isEmpty(u_authority));
    assertFalse(isMicroForm(u_authority));
    assertTrue(isLongForm(u_authority));
}

// Test create a remote uAuthority that supports long UUris null device.
static void testLongRemoteEmptyDevice() {
    std::string domain = "myvin";
    std::string device;
    auto u_authority = BuildUAuthority().setName(device, domain).build();
    assertFalse(u_authority.has_ip());
    assertFalse(u_authority.has_id());
    assertTrue(u_authority.has_name());
    assertEquals("myvin", u_authority.name());
    assertFalse(u_authority.has_ip());
    assertFalse(isLocal(u_authority));
    assertTrue(isRemote(u_authority));
    assertFalse(isResolved(u_authority));
    assertFalse(isEmpty(u_authority));
    assertFalse(isMicroForm(u_authority));
    assertTrue(isLongForm(u_authority));
}

// Test create a remote uAuthority that supports long UUris missing device.
static void testLongUriBlankDevice() {
    std::string device = " ";
    std::string domain = "myvin";
    auto u_authority  = BuildUAuthority().setName(device, domain).build();
    assertTrue(u_authority.has_name());
    assertEquals(domain, u_authority.name());
    assertFalse(u_authority.has_ip());
    assertFalse(isLocal(u_authority));
    assertTrue(isRemote(u_authority));
    assertFalse(isResolved(u_authority));
    assertFalse(isEmpty(u_authority));
    assertFalse(isMicroForm(u_authority));
    assertTrue(isLongForm(u_authority));
}

// Test create a remote uAuthority that supports long UUris null domain.
static void testLongUriEmptyDomain() {
    std::string device = "vcu";
    std::string domain;
    auto u_authority = BuildUAuthority().setName(device, domain).build();
    assertTrue(u_authority.has_name());
    assertEquals(device, u_authority.name());
    assertFalse(u_authority.has_ip());
    assertFalse(isLocal(u_authority));
    assertTrue(isRemote(u_authority));
    assertFalse(isResolved(u_authority));
    assertFalse(isEmpty(u_authority));
    assertFalse(isMicroForm(u_authority));
    assertTrue(isLongForm(u_authority));
}

// Test create a remote uAuthority that supports micro UUris.
static void testMicroUriUAuthority() {
    std::string address = "127.0.0.1";
    auto u_authority = BuildUAuthority().setIp(address).build();
    assertFalse(u_authority.has_name());
    assertTrue(u_authority.has_ip());
    assertEquals(address, u_authority.ip());
    assertFalse(isLocal(u_authority));
    assertTrue(isRemote(u_authority));
    assertFalse(isResolved(u_authority));
    assertFalse(isEmpty(u_authority));
    assertTrue(isMicroForm(u_authority));
    assertFalse(isLongForm(u_authority));
}

// Test create a remote uAuthority that supports micro UUris with null address.
//since local micro UURI is not containing address, so this test case is not valid.
static void testMicroUriEmptyAddress() {
    auto u_authority1 = BuildUAuthority().setName("").build();
    assertTrue(u_authority1.remote_case() == uprotocol::v1::UAuthority::REMOTE_NOT_SET);
    assertTrue(isEmpty(u_authority1));
}



Ensure(UAuthority, all_tests) {
    testToString();
    testToStringCaseSensitivity();
    testEmptyUAuthority();
    testLocalUAuthority();
    testLongRemoteUAuthority();
    testLongRemoteEmptyDevice();
    testLongUriBlankDevice();
    testLongUriEmptyDomain();
    testMicroUriUAuthority();
    testMicroUriEmptyAddress();
}

auto main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv) -> int {
    TestSuite* suite = create_test_suite();

    add_test_with_context(suite, UAuthority, all_tests);

    return run_test_suite(suite, create_text_reporter());
}