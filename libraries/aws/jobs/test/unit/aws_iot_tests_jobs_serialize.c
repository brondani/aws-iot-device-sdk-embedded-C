/*
 * AWS IoT Jobs V1.0.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file aws_iot_tests_jobs_serialize.c
 * @brief Tests for the Jobs JSON functions.
 */

/* The config header is always included first. */
#include "iot_config.h"

/* Standard includes. */
#include <string.h>

/* SDK initialization include. */
#include "iot_init.h"

/* Jobs internal include. */
#include "private/aws_iot_jobs_internal.h"

/* Test framework includes. */
#include "unity_fixture.h"

/* JSON utilities include. */
#include "aws_iot_doc_parser.h"

/*-----------------------------------------------------------*/

/**
 * @brief Test group for Jobs Serialize tests.
 */
TEST_GROUP( Jobs_Unit_Serialize );

/*-----------------------------------------------------------*/

/**
 * @brief Test setup for Jobs Serialize tests.
 */
TEST_SETUP( Jobs_Unit_Serialize )
{
    /* Initialize SDK. */
    TEST_ASSERT_EQUAL_INT( true, IotSdk_Init() );

    /* Initialize the Jobs library. */
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, AwsIotJobs_Init( 0 ) );
}

/*-----------------------------------------------------------*/

/**
 * @brief Test tear down for Jobs Serialize tests.
 */
TEST_TEAR_DOWN( Jobs_Unit_Serialize )
{
    /* Clean up libraries. */
    AwsIotJobs_Cleanup();
    IotSdk_Cleanup();
}

/*-----------------------------------------------------------*/

/**
 * @brief Test group runner for Jobs Serialize tests.
 */
TEST_GROUP_RUNNER( Jobs_Unit_Serialize )
{
    RUN_TEST_CASE( Jobs_Unit_Serialize, SerializeGetPending );
    RUN_TEST_CASE( Jobs_Unit_Serialize, SerializeStartNextClientToken );
    RUN_TEST_CASE( Jobs_Unit_Serialize, SerializeStartNextStepTimeout );
    RUN_TEST_CASE( Jobs_Unit_Serialize, SerializeStartNextStatusDetails );
    RUN_TEST_CASE( Jobs_Unit_Serialize, SerializeDescribe );
    RUN_TEST_CASE( Jobs_Unit_Serialize, SerializeUpdateStatus );
    RUN_TEST_CASE( Jobs_Unit_Serialize, SerializeUpdateNumbers );
    RUN_TEST_CASE( Jobs_Unit_Serialize, SerializeUpdateFlags );
    RUN_TEST_CASE( Jobs_Unit_Serialize, ParseErrorResponse );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests generation of JSON documents for GET PENDING requests.
 */
TEST( Jobs_Unit_Serialize, SerializeGetPending )
{
    AwsIotJobsError_t status = AWS_IOT_JOBS_STATUS_PENDING;
    _jobsOperation_t operation = { .link = { 0 } };
    AwsIotJobsRequestInfo_t requestInfo = AWS_IOT_JOBS_REQUEST_INFO_INITIALIZER;
    const char * pClientToken = NULL;
    size_t clientTokenLength = 0;

    /* Generate request with autogenerated client token. */
    status = _AwsIotJobs_GenerateJsonRequest( JOBS_GET_PENDING,
                                              &requestInfo,
                                              NULL,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that a client token is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            AWS_IOT_CLIENT_TOKEN_KEY,
                                                            AWS_IOT_CLIENT_TOKEN_KEY_LENGTH,
                                                            &pClientToken,
                                                            &clientTokenLength ) );
    TEST_ASSERT_EQUAL_PTR( pClientToken, operation.pClientToken );
    TEST_ASSERT_EQUAL( clientTokenLength, operation.clientTokenLength );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request with custom client token. */
    requestInfo.pClientToken = "test";
    requestInfo.clientTokenLength = 4;
    status = _AwsIotJobs_GenerateJsonRequest( JOBS_GET_PENDING,
                                              &requestInfo,
                                              NULL,
                                              &operation );

    /* Check that the custom client token is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            AWS_IOT_CLIENT_TOKEN_KEY,
                                                            AWS_IOT_CLIENT_TOKEN_KEY_LENGTH,
                                                            &pClientToken,
                                                            &clientTokenLength ) );
    TEST_ASSERT_EQUAL_PTR( pClientToken, operation.pClientToken );
    TEST_ASSERT_EQUAL( 6, operation.clientTokenLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "\"test\"", operation.pClientToken, 6 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests generation of client token in JSON documents for START NEXT
 * requests.
 */
TEST( Jobs_Unit_Serialize, SerializeStartNextClientToken )
{
    AwsIotJobsError_t status = AWS_IOT_JOBS_STATUS_PENDING;
    _jobsOperation_t operation = { .link = { 0 } };
    AwsIotJobsRequestInfo_t requestInfo = AWS_IOT_JOBS_REQUEST_INFO_INITIALIZER;
    AwsIotJobsUpdateInfo_t updateInfo = AWS_IOT_JOBS_UPDATE_INFO_INITIALIZER;
    _jsonRequestContents_t requestContents = { 0 };
    const char * pJsonValue = NULL;
    size_t jsonValueLength = 0;

    requestContents.pUpdateInfo = &updateInfo;

    /* Generate request with only autogenerated client token. */
    status = _AwsIotJobs_GenerateJsonRequest( JOBS_START_NEXT,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that a client token is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            AWS_IOT_CLIENT_TOKEN_KEY,
                                                            AWS_IOT_CLIENT_TOKEN_KEY_LENGTH,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL_PTR( pJsonValue, operation.pClientToken );
    TEST_ASSERT_EQUAL( jsonValueLength, operation.clientTokenLength );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request with custom client token. */
    requestInfo.pClientToken = "test";
    requestInfo.clientTokenLength = 4;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_START_NEXT,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that the custom client token is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            AWS_IOT_CLIENT_TOKEN_KEY,
                                                            AWS_IOT_CLIENT_TOKEN_KEY_LENGTH,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL_PTR( pJsonValue, operation.pClientToken );
    TEST_ASSERT_EQUAL( 6, operation.clientTokenLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "\"test\"", pJsonValue, 6 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests generation of step timeout in JSON documents for START NEXT
 * requests.
 */
TEST( Jobs_Unit_Serialize, SerializeStartNextStepTimeout )
{
    AwsIotJobsError_t status = AWS_IOT_JOBS_STATUS_PENDING;
    _jobsOperation_t operation = { .link = { 0 } };
    AwsIotJobsRequestInfo_t requestInfo = AWS_IOT_JOBS_REQUEST_INFO_INITIALIZER;
    AwsIotJobsUpdateInfo_t updateInfo = AWS_IOT_JOBS_UPDATE_INFO_INITIALIZER;
    _jsonRequestContents_t requestContents = { 0 };
    const char * pJsonValue = NULL;
    size_t jsonValueLength = 0;

    requestContents.pUpdateInfo = &updateInfo;

    /* Step timeout should not be present if not provided.  */
    status = _AwsIotJobs_GenerateJsonRequest( JOBS_START_NEXT,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    TEST_ASSERT_EQUAL_INT( false, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                             operation.jobsRequestLength,
                                                             "stepTimeoutInMinutes",
                                                             20,
                                                             &pJsonValue,
                                                             &jsonValueLength ) );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request using the "cancel timeout" value. */
    updateInfo.stepTimeoutInMinutes = AWS_IOT_JOBS_CANCEL_TIMEOUT;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_START_NEXT,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that the step timeout is -1. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "stepTimeoutInMinutes",
                                                            20,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 2, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "-1", pJsonValue, 2 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request with step timeout. */
    updateInfo.stepTimeoutInMinutes = 3600;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_START_NEXT,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that the step timeout is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "stepTimeoutInMinutes",
                                                            20,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 4, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "3600", pJsonValue, 4 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests generation of status details in JSON documents for START NEXT
 * requests.
 */
TEST( Jobs_Unit_Serialize, SerializeStartNextStatusDetails )
{
    AwsIotJobsError_t status = AWS_IOT_JOBS_STATUS_PENDING;
    _jobsOperation_t operation = { .link = { 0 } };
    AwsIotJobsRequestInfo_t requestInfo = AWS_IOT_JOBS_REQUEST_INFO_INITIALIZER;
    AwsIotJobsUpdateInfo_t updateInfo = AWS_IOT_JOBS_UPDATE_INFO_INITIALIZER;
    _jsonRequestContents_t requestContents = { 0 };
    const char * pJsonValue = NULL;
    size_t jsonValueLength = 0;

    requestContents.pUpdateInfo = &updateInfo;

    /* Generate request with status details. */
    updateInfo.pStatusDetails = "{\"status\":0}";
    updateInfo.statusDetailsLength = 12;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_START_NEXT,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that the status details are present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "statusDetails",
                                                            13,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 12, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "{\"status\":0}", pJsonValue, 12 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request with status details, step timeout, and custom client token. */
    updateInfo.stepTimeoutInMinutes = 10080;
    requestInfo.pClientToken = "test";
    requestInfo.clientTokenLength = 4;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_START_NEXT,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that the status details are present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "statusDetails",
                                                            13,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 12, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "{\"status\":0}", pJsonValue, 12 );

    /* Check that step timeout is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "stepTimeoutInMinutes",
                                                            20,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 5, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "10080", pJsonValue, 5 );

    /* Check that the custom client token is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            AWS_IOT_CLIENT_TOKEN_KEY,
                                                            AWS_IOT_CLIENT_TOKEN_KEY_LENGTH,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL_PTR( pJsonValue, operation.pClientToken );
    TEST_ASSERT_EQUAL( 6, operation.clientTokenLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "\"test\"", pJsonValue, 6 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests generation of status details in JSON documents for DESCRIBE
 * requests.
 */
TEST( Jobs_Unit_Serialize, SerializeDescribe )
{
    AwsIotJobsError_t status = AWS_IOT_JOBS_STATUS_PENDING;
    _jobsOperation_t operation = { .link = { 0 } };
    AwsIotJobsRequestInfo_t requestInfo = AWS_IOT_JOBS_REQUEST_INFO_INITIALIZER;
    _jsonRequestContents_t requestContents = { 0 };
    const char * pJsonValue = NULL;
    size_t jsonValueLength = 0;

    /* Generate request with only autogenerated client token. */
    requestContents.describe.includeJobDocument = true;
    requestContents.describe.executionNumber = AWS_IOT_JOBS_NO_EXECUTION_NUMBER;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_DESCRIBE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that a client token is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            AWS_IOT_CLIENT_TOKEN_KEY,
                                                            AWS_IOT_CLIENT_TOKEN_KEY_LENGTH,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL_PTR( pJsonValue, operation.pClientToken );
    TEST_ASSERT_EQUAL( jsonValueLength, operation.clientTokenLength );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request with all parameters. */
    requestContents.describe.includeJobDocument = false;
    requestContents.describe.executionNumber = 555555555;
    requestInfo.pClientToken = "test";
    requestInfo.clientTokenLength = 4;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_DESCRIBE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that the custom client token is present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            AWS_IOT_CLIENT_TOKEN_KEY,
                                                            AWS_IOT_CLIENT_TOKEN_KEY_LENGTH,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL_PTR( pJsonValue, operation.pClientToken );
    TEST_ASSERT_EQUAL( 6, operation.clientTokenLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "\"test\"", pJsonValue, 6 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests generation of status in JSON documents for UPDATE
 * requests.
 */
TEST( Jobs_Unit_Serialize, SerializeUpdateStatus )
{
    uint32_t i = 0;
    AwsIotJobsError_t status = AWS_IOT_JOBS_STATUS_PENDING;
    _jobsOperation_t operation = { .link = { 0 } };
    AwsIotJobsRequestInfo_t requestInfo = AWS_IOT_JOBS_REQUEST_INFO_INITIALIZER;
    AwsIotJobsUpdateInfo_t updateInfo = AWS_IOT_JOBS_UPDATE_INFO_INITIALIZER;
    _jsonRequestContents_t requestContents = { 0 };
    const char * pJsonValue = NULL;
    size_t jsonValueLength = 0;

    /* Valid statuses to check. */
    const AwsIotJobState_t pValidStatus[ 4 ] =
    {
        AWS_IOT_JOB_STATE_IN_PROGRESS, AWS_IOT_JOB_STATE_FAILED,
        AWS_IOT_JOB_STATE_SUCCEEDED,   AWS_IOT_JOB_STATE_REJECTED
    };

    const char * const pValidStatusStrings[ 4 ] =
    {
        "\"IN_PROGRESS\"", "\"FAILED\"", "\"SUCCEEDED\"", "\"REJECTED\""
    };

    requestContents.pUpdateInfo = &updateInfo;

    /* Generate request documents with valid statuses. */
    for( i = 0; i < ( sizeof( pValidStatus ) / sizeof( pValidStatus[ 0 ] ) ); i++ )
    {
        updateInfo.newStatus = pValidStatus[ i ];

        status = _AwsIotJobs_GenerateJsonRequest( JOBS_UPDATE,
                                                  &requestInfo,
                                                  &requestContents,
                                                  &operation );
        TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

        /* Check the status value. */
        TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                                operation.jobsRequestLength,
                                                                "status",
                                                                6,
                                                                &pJsonValue,
                                                                &jsonValueLength ) );
        TEST_ASSERT_EQUAL( strlen( pValidStatusStrings[ i ] ), jsonValueLength );
        TEST_ASSERT_EQUAL_STRING_LEN( pValidStatusStrings[ i ], pJsonValue, jsonValueLength );

        /* Check that the step timeout is not present. */
        TEST_ASSERT_EQUAL_INT( false, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                                 operation.jobsRequestLength,
                                                                 "stepTimeoutInMinutes",
                                                                 20,
                                                                 &pJsonValue,
                                                                 &jsonValueLength ) );

        /* Check that a client token is present. */
        TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                                operation.jobsRequestLength,
                                                                AWS_IOT_CLIENT_TOKEN_KEY,
                                                                AWS_IOT_CLIENT_TOKEN_KEY_LENGTH,
                                                                &pJsonValue,
                                                                &jsonValueLength ) );
        TEST_ASSERT_EQUAL_PTR( pJsonValue, operation.pClientToken );
        TEST_ASSERT_EQUAL( jsonValueLength, operation.clientTokenLength );

        /* Free request document generated by serializer. */
        AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
    }

    /* Generate request with status details. */
    updateInfo.pStatusDetails = "{\"status\":0}";
    updateInfo.statusDetailsLength = 12;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_UPDATE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that the status details are present. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "statusDetails",
                                                            13,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 12, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "{\"status\":0}", pJsonValue, 12 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests generation of numbers in JSON documents for UPDATE
 * requests.
 */
TEST( Jobs_Unit_Serialize, SerializeUpdateNumbers )
{
    AwsIotJobsError_t status = AWS_IOT_JOBS_STATUS_PENDING;
    _jobsOperation_t operation = { .link = { 0 } };
    AwsIotJobsRequestInfo_t requestInfo = AWS_IOT_JOBS_REQUEST_INFO_INITIALIZER;
    AwsIotJobsUpdateInfo_t updateInfo = AWS_IOT_JOBS_UPDATE_INFO_INITIALIZER;
    _jsonRequestContents_t requestContents = { 0 };
    const char * pJsonValue = NULL;
    size_t jsonValueLength = 0;

    requestContents.pUpdateInfo = &updateInfo;

    /* Generate request document with expected version. */
    updateInfo.expectedVersion = 1;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_UPDATE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check the expected version. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "expectedVersion",
                                                            15,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 3, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "\"1\"", pJsonValue, 3 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request document with execution number. */
    updateInfo.executionNumber = 555555555;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_UPDATE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check the execution number. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "executionNumber",
                                                            15,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 9, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "555555555", pJsonValue, 9 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request document with step timeout. */
    updateInfo.stepTimeoutInMinutes = 10080;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_UPDATE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check the step timeout. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "stepTimeoutInMinutes",
                                                            20,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 5, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "10080", pJsonValue, 5 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate request using the "cancel timeout" value. */
    updateInfo.stepTimeoutInMinutes = AWS_IOT_JOBS_CANCEL_TIMEOUT;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_UPDATE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that step timeout is -1. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "stepTimeoutInMinutes",
                                                            20,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 2, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "-1", pJsonValue, 2 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests generation of flags in JSON documents for UPDATE
 * requests.
 */
TEST( Jobs_Unit_Serialize, SerializeUpdateFlags )
{
    AwsIotJobsError_t status = AWS_IOT_JOBS_STATUS_PENDING;
    _jobsOperation_t operation = { .link = { 0 } };
    AwsIotJobsRequestInfo_t requestInfo = AWS_IOT_JOBS_REQUEST_INFO_INITIALIZER;
    AwsIotJobsUpdateInfo_t updateInfo = AWS_IOT_JOBS_UPDATE_INFO_INITIALIZER;
    _jsonRequestContents_t requestContents = { 0 };
    const char * pJsonValue = NULL;
    size_t jsonValueLength = 0;

    requestContents.pUpdateInfo = &updateInfo;

    /* Generate a request without any flags set. */
    requestInfo.pClientToken = "test";
    requestInfo.clientTokenLength = 4;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_UPDATE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that no flags are present. */
    TEST_ASSERT_EQUAL_INT( false, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                             operation.jobsRequestLength,
                                                             "includeJobDocument",
                                                             18,
                                                             &pJsonValue,
                                                             &jsonValueLength ) );
    TEST_ASSERT_EQUAL_INT( false, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                             operation.jobsRequestLength,
                                                             "includeJobExecutionState",
                                                             24,
                                                             &pJsonValue,
                                                             &jsonValueLength ) );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );

    /* Generate a request with flags set. */
    updateInfo.includeJobDocument = true;
    updateInfo.includeJobExecutionState = true;

    status = _AwsIotJobs_GenerateJsonRequest( JOBS_UPDATE,
                                              &requestInfo,
                                              &requestContents,
                                              &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_SUCCESS, status );

    /* Check that flags are set. */
    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "includeJobDocument",
                                                            18,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 4, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "true", pJsonValue, 4 );

    TEST_ASSERT_EQUAL_INT( true, AwsIotDocParser_FindValue( operation.pJobsRequest,
                                                            operation.jobsRequestLength,
                                                            "includeJobExecutionState",
                                                            24,
                                                            &pJsonValue,
                                                            &jsonValueLength ) );
    TEST_ASSERT_EQUAL( 4, jsonValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "true", pJsonValue, 4 );

    /* Free request document generated by serializer. */
    AwsIotJobs_FreeString( ( void * ) operation.pJobsRequest );
}

/*-----------------------------------------------------------*/

/**
 * @brief Tests parsing of Jobs error responses.
 */
TEST( Jobs_Unit_Serialize, ParseErrorResponse )
{
    size_t i = 0;
    _jobsOperation_t operation = { .link = { 0 } };

    /* Test for failure to allocate memory for response. */
    #if IOT_TEST_NO_MALLOC_OVERRIDES != 1
        operation.status = AWS_IOT_JOBS_STATUS_PENDING;
        operation.flags = AWS_IOT_JOBS_FLAG_WAITABLE;
        operation.mallocResponse = IotTest_Malloc;

        UnityMalloc_MakeMallocFailAfterCount( 0 );

        _AwsIotJobs_ParseResponse( AWS_IOT_ACCEPTED, "{}", 2, &operation );
        TEST_ASSERT_EQUAL( AWS_IOT_JOBS_NO_MEMORY, operation.status );
        UnityMalloc_MakeMallocFailAfterCount( -1 );
        operation.flags = 0;
    #endif /* if IOT_TEST_NO_MALLOC_OVERRIDES != 1 */

    /* Test parsing of error responses. */
    const char * pErrorResponses[] =
    {
        "{\"code\": \"InvalidTopic\"}",
        "{\"code\": \"InvalidJson\"}",
        "{\"code\": \"InvalidRequest\"}",
        "{\"code\": \"InvalidStateTransition\"}",
        "{\"code\": \"ResourceNotFound\"}",
        "{\"code\": \"VersionMismatch\"}",
        "{\"code\": \"InternalError\"}",
        "{\"code\": \"RequestThrottled\"}",
        "{\"code\": \"TerminalStateReached\"}"
    };
    const AwsIotJobsError_t pExpectedResponse[] =
    {
        AWS_IOT_JOBS_INVALID_TOPIC,
        AWS_IOT_JOBS_INVALID_JSON,
        AWS_IOT_JOBS_INVALID_REQUEST,
        AWS_IOT_JOBS_INVALID_STATE,
        AWS_IOT_JOBS_NOT_FOUND,
        AWS_IOT_JOBS_VERSION_MISMATCH,
        AWS_IOT_JOBS_INTERNAL_ERROR,
        AWS_IOT_JOBS_THROTTLED,
        AWS_IOT_JOBS_TERMINAL_STATE
    };

    for( i = 0; i < ( sizeof( pErrorResponses ) / sizeof( pErrorResponses[ 0 ] ) ); i++ )
    {
        operation.status = AWS_IOT_JOBS_STATUS_PENDING;

        _AwsIotJobs_ParseResponse( AWS_IOT_REJECTED,
                                   pErrorResponses[ i ],
                                   strlen( pErrorResponses[ i ] ),
                                   &operation );
        TEST_ASSERT_EQUAL( pExpectedResponse[ i ], operation.status );
    }

    /* Test parsing of invalid code. */
    operation.status = AWS_IOT_JOBS_STATUS_PENDING;
    _AwsIotJobs_ParseResponse( AWS_IOT_REJECTED,
                               "{\"code\": \"NotAnErrorCodeNotAnErrorCode\"}",
                               40,
                               &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_BAD_RESPONSE, operation.status );

    operation.status = AWS_IOT_JOBS_STATUS_PENDING;
    _AwsIotJobs_ParseResponse( AWS_IOT_REJECTED,
                               "{}",
                               2,
                               &operation );
    TEST_ASSERT_EQUAL( AWS_IOT_JOBS_BAD_RESPONSE, operation.status );
}

/*-----------------------------------------------------------*/