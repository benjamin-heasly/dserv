#
# VARIANTS
#   hello demo
#
# DESCRIPTION
#   variant dictionary
#

namespace eval hello::demo {    
    variable variants {
        default {
            description "default demo"
            loader_proc basic_hello
    	    loader_options {}
        }
    }

    proc variants_init { s } {
        $s add_method basic_hello { } { }
    }
}
