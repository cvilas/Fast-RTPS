# Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if(NOT ((MSVC OR MSVC_IDE) AND EPROSIMA_INSTALLER))
    include(${PROJECT_SOURCE_DIR}/cmake/common/gtest.cmake)
    check_gtest()

    if(GTEST_FOUND)
        include_directories(${ASIO_INCLUDE_DIR})

        find_package(Threads REQUIRED)

        include_directories(${ASIO_INCLUDE_DIR})

        set(NETWORKFACTORYTESTS_SOURCE
            NetworkFactoryTests.cpp
            mock/MockTransport.cpp

            ${PROJECT_SOURCE_DIR}/src/cpp/dds/log/Log.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/dds/log/StdoutConsumer.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/common/Time_t.cpp

            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/ChannelResource.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/UDPChannelResource.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/UDPTransportInterface.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/UDPv4Transport.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/UDPv6Transport.cpp

            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/tcp/RTCPMessageManager.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/tcp/TCPControlMessage.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPChannelResource.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPChannelResourceBasic.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPAcceptor.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPAcceptorBasic.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPTransportInterface.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPv4Transport.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPv6Transport.cpp

            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/messages/RTPSMessageCreator.cpp

            ${PROJECT_SOURCE_DIR}/src/cpp/dds/core/policy/ParameterList.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/dds/core/policy/ParameterTypes.cpp

            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/resources/ResourceEvent.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/resources/TimedEvent.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/resources/TimedEventImpl.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/utils/TimedConditionVariable.cpp

            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/network/NetworkFactory.cpp

            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/utils/IPFinder.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/utils/IPLocator.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/utils/md5.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/utils/System.cpp
        )

        if(TLS_FOUND)
            set(NETWORKFACTORYTESTS_SOURCE
                ${NETWORKFACTORYTESTS_SOURCE}
                ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPChannelResourceSecure.cpp
                ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/transport/TCPAcceptorSecure.cpp
                ${PROJECT_SOURCE_DIR}/src/cpp/fastrtps_deprecated/security/OpenSSLInit.cpp
            )
        endif()

        include_directories(mock/)

        add_executable(NetworkFactoryTests ${NETWORKFACTORYTESTS_SOURCE})
        target_compile_definitions(NetworkFactoryTests PRIVATE FASTRTPS_NO_LIB)
        target_include_directories(NetworkFactoryTests PRIVATE
            ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/ParticipantProxyData
            ${PROJECT_SOURCE_DIR}/test/mock/dds/QosPolicies
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/MessageReceiver
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/ReceiverResource
            ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
            ${PROJECT_SOURCE_DIR}/src/cpp
            )
        target_link_libraries(NetworkFactoryTests fastcdr foonathan_memory
            ${GTEST_LIBRARIES} ${MOCKS}
            $<$<BOOL:${TLS_FOUND}>:OpenSSL::SSL$<SEMICOLON>OpenSSL::Crypto>
            ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})

        if(WIN32)
            add_definitions(-D_WIN32_WINNT=0x0601)
            target_link_libraries(NetworkFactoryTests IPHLPAPI shlwapi) # Later so mocks have precedence
        endif()

        add_gtest(NetworkFactoryTests SOURCES ${NETWORKFACTORYTESTS_SOURCE})

    endif()
endif()
