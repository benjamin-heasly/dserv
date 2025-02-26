##
##  NAME
##    hello.tcl
##
##  DECRIPTION
##    System for learning about writing systems
##

package require ess

# Encapsulate variables and commands specific to our "hello" system.
# https://www.tcl-lang.org/man/tcl8.5/TclCmd/namespace.htm#M24
namespace eval hello {

    # This create procedure will return a "System" object that defines the "hello" system.
    proc create {} {

		# The ess System class is like a customizable template for creating state systems.
		# ::ess::create_system creates a fresh System template for us to customize.
		# [namespace tail [namespace current]] gets a short name for our encapsulating namespace: "hello".
		set sys [::ess::create_system [namespace tail [namespace current]]]

		# Choose a version so we can track changes over time, compatibility, etc.	
		$sys set_version 1.0

		######################################################################
		#                          System Parameters                         #
		######################################################################

		# Systems can have custom parameters for controlling trial types, block structure, etc.
		# These are configurable for each session, for example via essgui.

		# This demo will have one parameter for the time interval between blinks.
		# Here's how we add the parameter to our System object, "sys":
		#	we choose a name, blink_interval
		#	we choose a value, 1000 (miliseconds)
		#	we choose a parameter type, time (could be ANY, TIME, VARIABLE, SUBJECT, STORAGE, or STIM)
		#	we choose a data type, int (integer)
		$sys add_param blink_interval	1000	time 	int
		
		# Systems can also have internal variables for keeping track of state.
		# These can change over time, but are not configurable from the outside.

		# This demo will have one internal variable that counts down blinks.
		# For variables:
		#	we choose a name, blink_count
		#	we choose an initial value, 10
		$sys add_variable blink_count	10

		######################################################################
		#                            System States                           #
		######################################################################

		# State systems have -- states!
		# We declare the starting and other possible states explicitly, up front, by name.
		# In addition to a name, we can declare for each state:
		#	- actions that can happen as the world changes
		#	- the posible transitions to other states
	
		# Our State system should one well-defined start state, probably always want to use "start".
		$sys set_start start

		# This demo will declare its start state with no actions.
		# It will immediately transition to another state, "setup".
		#
		# start
		#
		$sys add_state start {} { return setup }

		# The "setup" state will bring the system back to a consistent, initial state.
		# The after a timer expires, the system will transition to the "green" state.
		# When we add_action or add_transition, this implicitly adds a stat with the same name.
		# The commands timerTick, timerExpired, and now are part of dserv.
		#	-Have/want reference docs for commands like these?
		$sys add_action setup {
			::ess::evt_put SYSTEM_STATE RUNNING [now]
			set blink_count 10
			timerTick $blink_interval
		}
		$sys add_transition setup {
			if { [timerExpired] } { return green }
		}

		# The "green" state will remain until a timer expires, then transition to "brown".
		# But when the blink_count reaches 0, it will transition to the end.
		#
		# green
		#
		$sys add_action green {
			my green_screen
			timerTick $blink_interval
			::ess::evt_put STIMTYPE STIMID [now] 1
		}
		$sys add_transition green {
			if { [timerExpired] } {
				if { [my count_down] } {
					return end
				} else {
					return brown
				}
			}
		}

		# The "brown" state will remain until a timer expires, then transition back to "green".
		# But when the blink_count reaches 0, it will transition to the end.
		#
		# start_delay
		#
		$sys add_action brown {
			my brown_screen
			timerTick $blink_interval
			::ess::evt_put STIMTYPE STIMID [now] 2
		}
		$sys add_transition brown {
			if { [timerExpired] } {
				if { [my count_down] } {
					return end
				} else {
					return green
				}
			}
		}

		# Our State system should one well-defined end state, probably always want to use "start".
		# For this demo the end state will have no custom behavior.
		#
		# end
		# 
		$sys set_end {}
	
		######################################################################
		#                         System Callbacks                           #
		######################################################################

		# State systems can have callbacks.
		# These are custom "hook" commands that will be called when certain system events happen.
		
		$sys set_init_callback {
			::ess::init
		}
		
		$sys set_deinit_callback {}
		
		$sys set_reset_callback {
			set blink_count 10
		}
		
		$sys set_start_callback {
			set blink_count 10
		}
		
		$sys set_quit_callback {}
		
		$sys set_end_callback {}
		
		$sys set_file_open_callback {}
		
		$sys set_file_close_callback {}
		
		$sys set_subject_callback {}
		
		######################################################################
		#                          System Methods                            #
		######################################################################

		# Systems can have internal helper methods that we can call from state actions and transition methods.
		
		# Methods above call call this using [my count_down].
		$sys add_method count_down {} {
			set blink_count [expr {$blink_count - 1}]
			if { $blink_count > 0 } {
				return 0
			} else {
				return 1
			}
		}

		# Return our new, customized System object, ready for use.
		return $sys
    }
}
