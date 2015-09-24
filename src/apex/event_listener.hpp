//  Copyright (c) 2014 University of Oregon
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef APEX_EVENTLISTENER_H
#define APEX_EVENTLISTENER_H

#include <string>
#include <memory>
#include "apex_types.h"
#include "profiler.hpp"

namespace apex {

/* Class for holding data relevant to generic event */

class event_data {
public:
  apex_event_type event_type_;
  int thread_id;
  void * data; /* generic data pointer */
  event_data() : thread_id(0), data(NULL) {};
  virtual ~event_data() {};
};

/* Classes for holding data relevant to specific events */

class timer_event_data : public event_data {
public:
  bool have_name;
  std::string * timer_name;
  apex_function_address function_address;
  std::shared_ptr<profiler> my_profiler;
  timer_event_data(const std::string &timer_name);
  timer_event_data(apex_function_address function_address);
  timer_event_data(std::shared_ptr<profiler> the_profiler);
  ~timer_event_data();
};

class node_event_data : public event_data {
public:
  int node_id;
  node_event_data(int node_id, int thread_id);
};

class sample_value_event_data : public event_data {
public:
  std::string * counter_name;
  double counter_value;
  bool is_counter;
  sample_value_event_data(int thread_id, std::string counter_name, double counter_value);
  ~sample_value_event_data();
};

class startup_event_data : public event_data {
public:
  int argc;
  char** argv;
  startup_event_data(int argc, char** argv);
};

class shutdown_event_data : public event_data {
public:
  int node_id;
  shutdown_event_data(int node_id, int thread_id);
};

class new_thread_event_data : public event_data {
public:
  std::string* thread_name;
  new_thread_event_data(std::string thread_name);
  ~new_thread_event_data();
};

class periodic_event_data : public event_data {
public:
  periodic_event_data();
};

class custom_event_data : public event_data {
public:
  custom_event_data(apex_event_type event_type, void * custom_data);
  ~custom_event_data();
};

/* Abstract class for creating an Event Listener class */

class event_listener
{
public:
  // virtual destructor
  virtual ~event_listener() {};
  // all methods in the interface that a handler has to override
  virtual void on_startup(startup_event_data &data) = 0;
  virtual void on_shutdown(shutdown_event_data &data) = 0;
  virtual void on_new_node(node_event_data &data) = 0;
  virtual void on_new_thread(new_thread_event_data &data) = 0;
  virtual void on_exit_thread(event_data &data) = 0;
  virtual bool on_start(apex_function_address function_address) = 0;
  virtual bool on_start(std::string *timer_name) = 0;
  virtual void on_stop(std::shared_ptr<profiler> p) = 0;
  virtual void on_yield(std::shared_ptr<profiler> p) = 0;
  virtual bool on_resume(apex_function_address function_address) = 0;
  virtual bool on_resume(std::string *timer_name) = 0;
  virtual void on_new_task(apex_function_address function_address, void * task_id) = 0;
  virtual void on_new_task(std::string *timer_name, void * task_id) = 0;
  virtual void on_sample_value(sample_value_event_data &data) = 0;
  virtual void on_periodic(periodic_event_data &data) = 0;
  virtual void on_custom_event(custom_event_data &data) = 0;
};

}

#endif // APEX_EVENTLISTENER_H
