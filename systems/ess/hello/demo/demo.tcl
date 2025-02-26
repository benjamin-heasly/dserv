#
# PROTOCOL
#   hello demo
#
# DESCRIPTION
#   Demonstrate the hello system 
#

namespace eval hello::demo {
    proc protocol_init { s } {
    	$s set_protocol [namespace tail [namespace current]]

        $s add_param rmt_host          $::ess::rmt_host   stim ipaddr

        $s set_protocol_init_callback {
            ::ess::init
            
            # open connection to rmt and upload ${protocol}_stim.tcl
            my configure_stim $rmt_host
        }

        # Would this make sense in the state system hello.tcl?
        $s set_end_callback {
            ::ess::evt_put SYSTEM_STATE STOPPED [now]
        }

        $s set_protocol_deinit_callback {
            rmtClose
        }
        
        $s set_reset_callback {
            rmtSend "setBackground 10 10 10"
        }
        
        $s set_quit_callback {
            rmtSend "setBackground 10 10 10"
        }

        $s add_method brown_screen {} {
            rmtSend "setBackground 128 64 10"
        }

        $s add_method green_screen {} {
            rmtSend "setBackground 10 128 10"
        }

	    return
    }
}
