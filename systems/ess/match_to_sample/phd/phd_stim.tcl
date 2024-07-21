# NAME
#   phd_stim.tcl
#
# DESCRIPTION
#   match_to_sample with visual and phd generated shapes
#
# REQUIRES
#   polygon
#   metagroup
#
# AUTHOR
#   DLS
#

if { [info var dservhost] == {} } { set dservhost localhost }
# connect to data server receive stimdg updates
package require qpcs
package require hex
package require sqlite3

qpcs::dsStimRegister $dservhost
qpcs::dsStimAddMatch $dservhost stimdg

setBackground 200 200 200

# to stimdg is sent as b64 encoded string, this proc unpacks into stim
proc readdg { args } {
    dg_fromString64 [lindex $args 4]
}

# this sets the callback upon receipt of stimdg
set ::dsCmds(stimdg) readdg

# simulate touchscreen using  mouse from stim2 (don't need Release)
namespace inscope :: {
    proc onMousePress {} {
	global dservhost
	dl_local coords [dl_create short $::MouseXPos $::MouseYPos]
	qpcs::dsSetData $dservhost mtouch/touchvals $coords
    }
}

#
# nexttrial
#    create a stimulus based on the hex "qrs" notation which contains triples
# speficying the location of "pins" to show
#
proc create_shape { qrs scale { pinsize 12 } } {
    set scale 0.2
    dl_local centers [dl_transpose [dl_unpack [::hex::to_pixel $qrs $scale]]]

    set s [polygon]
    dl_set centers $centers
    polyverts $s $centers:0 $centers:1
    polytype $s points
    polypointsize $s $pinsize
    polycolor $s 0 0 0 
    return $s
}

proc nexttrial { id } {
    glistInit 2
    resetObjList

    set trialtype [dl_get stimdg:trial_type $id]
    set scale [dl_get stimdg:choice_scale $id]
    set scale 1.0
    set choice_centers [dl_get stimdg:choice_centers $id]
    
    # add the visual sample for VV trials, no visual sample for HV trials
    if { $trialtype == "VV" } {
	set sample [create_shape stimdg:sample_qrs:$id $scale]
	glistAddObject $sample 0
    } else {
	set object_id [dl_get stimdg:sample_id $id]
#	set phd_command [list print haptic_on $object_id]
#	glistSetInitCmd $phd_command 0
    }
    
    # match choices
    set mg [metagroup]
    foreach i "0 1 2 3" {
	set s [create_shape stimdg:choice_qrs:$id:$i $scale]
	translateObj $s {*}[dl_tcllist stimdg:choice_centers:$id:$i]
	#scaleObj $s [dl_get stimdg:choice_scale $id]
	metagroupAdd $mg $s
    }
    glistAddObject $mg 1
}
    
proc sample_on {} {
    glistSetCurGroup 0
    glistSetVisible 1
    redraw
}

proc sample_off {} {
    glistSetVisible 0
    redraw
}

proc choices_on {} {
    glistSetCurGroup 1
    glistSetVisible 1
    redraw
}

proc choices_off {} {
    glistSetVisible 0
    redraw
}

proc reset { } {
    glistSetVisible 0; redraw;
}

proc clearscreen { } {
    glistSetVisible 0; redraw;
}




#
# extract qrs representation for a given shape
#
proc get_shape_qrs { db shapeID { displayID 51 } } {
    
    # extract qrs hex location for each pin
    set sqlcmd { SELECT q,r,s FROM pistonAddressesTable \
		     WHERE DisplayID=$displayID }
    set qrsvals [$db eval $sqlcmd]
    dl_local qrs [dl_reshape [dl_ilist {*}$qrsvals] - 3]

    # get pin bit representation for this shape
    set sqlcmd {SELECT hex(shapeOutline) FROM shapeTable \
		    WHERE shapeID=$shapeID and DisplayID=$displayID}
    set h [$db eval $sqlcmd]

    # unpack blob into 32 bits words then converted to bits
    set hex [join [lreverse [join [regexp -all -inline .. $h]]] ""]
    set word0 [string range $hex 0 7]
    set word1 [string range $hex 8 15]
    set word2 [string range $hex 16 23]
    set word3 [string range $hex 24 31]
    set bits [format %032b%032b%032b%032b \
		  0x${word0} 0x${word1} 0x${word2} 0x${word3}]
    set bits [string range $bits 2 end]

    # turn bitmask into list of 1s and 0s for each pin
    dl_local pins [dl_ilist {*}[join [regexp -all -inline . $bits]]]

    # pull qrs location for each pin that is on
    dl_local hexpos [dl_choose $qrs [dl_indices $pins]]

    # return list of hex positions (in qrs notation)
    dl_return $hexpos
}

proc get_shape_family { db family algo algoargs { n 4 } } {

    # find all shapes from family shapeFamily with familyAlgo = $algo
    set sqlcmd {
	SELECT shapeID from shapeTable
	WHERE shapeFamily=$family and familyAlgo=$algo and algoArgsCSV=$algoargs
	LIMIT 4
    }
    set shapes [$db eval $sqlcmd]

    return $shapes
}

proc test_stimdg { trial_type } {
    if { [dg_exists stimdg] } { dg_delete stimdg }
    dg_create stimdg

    # open the database
    set dbfile "/Users/sheinb/projects/graspomatic/Grasp3ShapesRyan.db"
    sqlite3 grasp_db $dbfile -readonly true
    grasp_db timeout 5000

    dl_set stimdg:family [dl_ilist]
    dl_set stimdg:distance [dl_ilist]
    dl_set stimdg:shape_ids [dl_llist]
    dl_set stimdg:trial_type [dl_slist]
    dl_set stimdg:sample_qrs [dl_llist]
    dl_set stimdg:choice_qrs [dl_llist]
    
    set sqlcmd { SELECT shapeFamily from shapeTable WHERE familyAlgo="parent" and displayID=51}
    set families [grasp_db eval $sqlcmd]
    dl_set stimdg:family [dl_ilist {*}$families]
    set nfamilies [dl_length stimdg:family]
    set nrep [expr ($nfamilies+3)/4]
    dl_local distance [dl_choose [dl_replicate [dl_shuffle "1 2 4 8"] $nrep] \
			   [dl_fromto 0 $nfamilies]]
    dl_set stimdg:distance $distance

    foreach f $families d [dl_tcllist $distance] {
	dl_local nex [dl_ilist]
	set shape_id_list [get_shape_family grasp_db $f add_pistons2 ${d}]
	dl_local shape_ids [dl_ilist {*}$shape_id_list]
	dl_append stimdg:shape_ids $shape_ids
	dl_local qrs [dl_llist]
	set sample_id [dl_pickone $shape_ids]

	# add sample qrs
	dl_append stimdg:sample_qrs [get_shape_qrs grasp_db $sample_id]

	# add choice qrs
	dl_local choice_qrs [dl_llist]
	foreach choice_id $shape_id_list {
	    dl_append $choice_qrs [get_shape_qrs grasp_db $choice_id]
	}
	dl_append stimdg:choice_qrs $choice_qrs
    }

    set n [dl_length stimdg:sample_qrs]
    dl_set stimdg:trial_type [dl_replicate [dl_slist $trial_type] $n]
        
    grasp_db close
    
    qpcs::dgPut localhost stimdg stimdg

    return stimdg
}

