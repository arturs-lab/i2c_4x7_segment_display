$fn=32;
layer_t=0.3;

include <../bolt_dim.scad>

// testing=1 normal=0
test=0;

// printing=1
print=0;

numpins=4;

wall_t=1.5;
gap=0.5;

// extrusion width
e_width=0.5;


pcb_x=32;
pcb_y=16;
pcb_t=2;

// height of tallest component on PCB
pcb_height=2;
// height of components on back side
pcb_back=2;

// spae reserved for components
pcb_back_x=30;
pcb_back_y=14;

disp_w=30;
disp_l=14;
disp_h=9;
disp_x=pcb_x/2;
disp_y=pcb_y/2;

hdr_w=(numpins+0)*2.54;
hdr_h=1*2.54;
hdr_l=10;
hdr_mnthole_offset=3;   // mount hole offset from right side of board
/*
// m3 screws
screw_d=3;
screw_l=10;
screw_head_d=5.5;
screw_head_l=3;
*/

/*
// #4 screws
screw_d=bolt_d_n4;
screw_l=12;
screw_head_d=bolt_head_d_n4;
screw_head_l=bolt_head_l_n4;
*/

// screws for plastic
screw_d=2.5;
screw_l=3/8*25.4;
screw_head_d=4.5;
screw_head_l=2;

peg_d=screw_d+2*wall_t;
peg_head_r=(screw_head_d)/2+wall_t;

enclosure_x=pcb_x+hdr_l-hdr_mnthole_offset+peg_head_r+4*wall_t;
enclosure_y=max(pcb_y+2*wall_t,hdr_w+peg_head_r*2+peg_d+2*gap);
enclosure_top_z=disp_h;
enclosure_bottom_z=pcb_back+pcb_t+wall_t;

screw_x=[peg_head_r,peg_head_r,enclosure_x-peg_head_r,enclosure_x-peg_head_r];
screw_y=[peg_head_r,enclosure_y-peg_head_r,peg_head_r,enclosure_y-peg_head_r];

pcb_offset=(enclosure_x-pcb_x)/2-(hdr_l-hdr_mnthole_offset)-wall_t;

echo ("enclosure_x=",enclosure_x);
echo ("enclosure_y=",enclosure_y);
echo ("hdr_w=",hdr_w);
echo ("peg_head_r=",peg_head_r*2);

//!screw_and_nut(nut_offset=5,screw_print_support=layer_t,nut_print_support=layer_t);
module screw(screw_d=screw_d,screw_l=10,screw_head_d=screw_head_d,screw_head_l=screw_head_l,screw_print_support=0){
    difference(){
        union(){
            translate([0,0,screw_l/2])cylinder(d=screw_d,h=screw_l,center=true);
            translate([0,0,-screw_head_l/2])cylinder(d=screw_head_d,h=screw_head_l+0.1,center=true);
        }
        if (screw_print_support>0)
            translate([0,0,screw_print_support/2])cylinder(d=screw_head_d,h=screw_print_support,center=true);
    }
}

module cube_r(x=enclosure_x,y=enclosure_y,z=enclosure_bottom_z,r=2){
    union(){
        cube([x-2*r,y,z],center=true);
        cube([x,y-2*r,z],center=true);
        for (i=[-1,1]){
            for (j=[-1,1]){
                translate([i*(x/2-r),j*(y/2-r),0]){
                    cylinder(r=r,h=z,center=true);
                }
            }
        }
    }
}

module pcb(){
	// mockup parts
	render(){
		//pcb
		cube([pcb_x,pcb_y,pcb_t],center=true);
		// header
		translate([(pcb_x+hdr_l)/2-hdr_mnthole_offset,0,(hdr_h+pcb_t)/2])cube([hdr_l,hdr_w,hdr_h],center=true);
		// go to top of board, lower left edge
        translate([-pcb_x/2,-pcb_y/2,pcb_height/2]){
			// display
//			translate([disp_x,disp_y,disp_h/2])cube([disp_w,disp_l,disp_h],center=true);
			translate([disp_x,disp_y,7.5/2])cube([disp_w,disp_l,7.5],center=true);
        }
        // mockup components
        translate([0,0,-(pcb_height+pcb_back)/2])cube([pcb_back_x,pcb_back_y,pcb_back],center=true);
    }
}

module enclosure_top(){
    difference(){
        union(){
            // main shell
            difference(){
                cube_r(x=enclosure_x,y=enclosure_y,z=enclosure_top_z,r=peg_head_r);
                translate([0,0,-wall_t/2])cube_r(x=enclosure_x-2*wall_t,y=enclosure_y-2*wall_t,z=enclosure_top_z-wall_t+0.1,r=peg_head_r-wall_t);
            }
            // display sleeve
            translate([pcb_offset,0,0]){
                cube([disp_w+wall_t+2*gap,disp_l+wall_t+2*gap,enclosure_top_z],center=true);
            }
            // screw pegs
            translate([-enclosure_x/2,-enclosure_y/2,0]){
                for (i=[0,1,2,3]){
                    translate([screw_x[i],screw_y[i],0]){
                        cylinder(d=peg_d,h=enclosure_top_z,center=true);
                    }
                }
            }
        }
        // display cutout
        translate([pcb_offset,0,0])
            cube([disp_w+2*gap,disp_l+2*gap,enclosure_top_z+0.1],center=true);
        // header cutout
        translate([(enclosure_x-hdr_l+0.1)/2,0,-(enclosure_top_z-hdr_h)/2])
            cube([hdr_l,hdr_w+2*gap,hdr_h+2*gap],center=true);
        // screw holes
        translate([-enclosure_x/2,-enclosure_y/2,-(enclosure_top_z/2+wall_t)]){
            for (i=[0,1,2,3]){
                translate([screw_x[i],screw_y[i],0]){
                    screw(screw_d=screw_d,screw_l=enclosure_top_z,screw_head_d=screw_head_d,screw_head_l=screw_head_l,screw_print_support=0);
                }
            }
        }
         if (test > 0 ) {
            translate([0,-enclosure_y+screw_y[0],0])
                cube([2*enclosure_x,enclosure_y,3*enclosure_top_z],center=true);
        }
   }
}

module enclosure_bottom(){
    difference(){
        union(){
            // main shell
            difference(){
                cube_r(x=enclosure_x,y=enclosure_y,z=enclosure_bottom_z,r=peg_head_r);
                translate([0,0,wall_t/2])cube_r(x=enclosure_x-2*wall_t,y=enclosure_y-2*wall_t,z=enclosure_bottom_z-wall_t+0.1,r=peg_head_r-wall_t);
            }
            // pcb support
            translate([pcb_offset,0,-pcb_t/2])difference(){
                cube([pcb_x,pcb_y,enclosure_bottom_z-pcb_t],center=true);
                translate([0,0,wall_t/2])cube([pcb_x-2,pcb_y-2,enclosure_bottom_z-pcb_t-wall_t+0.1],center=true);
            }
            // screw pegs
            translate([-enclosure_x/2,-enclosure_y/2,0]){
                for (i=[0,1,2,3]){
                    translate([screw_x[i],screw_y[i],0]){
                        translate([0,0,-(enclosure_bottom_z-screw_head_l-wall_t)/2])cylinder(r=peg_head_r,h=screw_head_l+wall_t,center=true);
                        cylinder(d=peg_d,h=enclosure_bottom_z,center=true);
                    }
                }
            }
        }
        // pcb
        translate([pcb_offset,0,(enclosure_bottom_z-pcb_t)/2]){
            cube([pcb_x,pcb_y,pcb_t+0.1],center=true);
        }
        // header cutout
        translate([(enclosure_x-hdr_l+0.1)/2,0,(enclosure_bottom_z)/2])
            cube([hdr_l,hdr_w+2*gap,hdr_h/2+gap],center=true);
        translate([-enclosure_x/2,-enclosure_y/2,-(enclosure_bottom_z)/2+screw_head_l]){
            for (i=[0,1,2,3]){
                translate([screw_x[i],screw_y[i],0]){
                    screw(screw_d=screw_d+1,screw_l=screw_l,screw_head_d=screw_head_d+1,screw_head_l=screw_head_l+1,screw_print_support=layer_t);
                }
            }
        }
         if (test > 0 ) {
            translate([0,-enclosure_y+screw_y[0],0])
                cube([2*enclosure_x,enclosure_y,3*enclosure_top_z],center=true);
        }
    }
}

module assembly(){
    test=1;
    difference(){
        union(){
            translate([0,0,enclosure_top_z/2])enclosure_top();
            translate([pcb_offset,0,-pcb_t/2])%pcb();
            translate([0,0,-enclosure_bottom_z/2])
                enclosure_bottom();
            translate([-enclosure_x/2,-enclosure_y/2,-enclosure_bottom_z+screw_head_l]){
                for (i=[0,1,2,3]){
                    translate([screw_x[i],screw_y[i],0]){
                        %screw(screw_d=screw_d,screw_l=screw_l,screw_head_d=screw_head_d,screw_head_l=screw_head_l);
                    }
                }
            }
        }
        if (test > 0 ) {
            translate([0,-enclosure_y+screw_y[0],0])
                cube([2*enclosure_x,enclosure_y,3*enclosure_top_z],center=true);
        }
    }
}

module display_led_4x7_enclosure(print=0){
    if (print==0) assembly();
    if (print==1) translate([0,0,enclosure_bottom_z/2])enclosure_bottom();
    if (print==2) translate([0,enclosure_y+2,enclosure_top_z/2])rotate([180,0,0])enclosure_top();
}

//cube_r(x=enclosure_x,y=enclosure_y,z=enclosure_bottom_z,r=2);
//enclosure_bottom();
//enclosure_top();


display_led_4x7_enclosure(0);
