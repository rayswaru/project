// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from bumperbot_msgs:srv/AddTwoints.idl
// generated code does not contain a copyright notice

#ifndef BUMPERBOT_MSGS__SRV__DETAIL__ADD_TWOINTS__STRUCT_H_
#define BUMPERBOT_MSGS__SRV__DETAIL__ADD_TWOINTS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/AddTwoints in the package bumperbot_msgs.
typedef struct bumperbot_msgs__srv__AddTwoints_Request
{
  int64_t a;
  int64_t b;
} bumperbot_msgs__srv__AddTwoints_Request;

// Struct for a sequence of bumperbot_msgs__srv__AddTwoints_Request.
typedef struct bumperbot_msgs__srv__AddTwoints_Request__Sequence
{
  bumperbot_msgs__srv__AddTwoints_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} bumperbot_msgs__srv__AddTwoints_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/AddTwoints in the package bumperbot_msgs.
typedef struct bumperbot_msgs__srv__AddTwoints_Response
{
  int64_t sum;
} bumperbot_msgs__srv__AddTwoints_Response;

// Struct for a sequence of bumperbot_msgs__srv__AddTwoints_Response.
typedef struct bumperbot_msgs__srv__AddTwoints_Response__Sequence
{
  bumperbot_msgs__srv__AddTwoints_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} bumperbot_msgs__srv__AddTwoints_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // BUMPERBOT_MSGS__SRV__DETAIL__ADD_TWOINTS__STRUCT_H_
