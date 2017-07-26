$fn=100;
include <rounded_square.scad>;


h_outer = 34;
w_outer = 34;
steg = 3;
h_inner = h_outer-(steg*2);
w_inner = w_outer-(steg*2);
rad_out = 9;
rad_in = 5;


//beer
dist_x = 420;
dist_y = 320;

// mate
//dist_x = 320;
//dist_y = 260;

dist_edge = 40;


module sensor_pad(){
  difference(){
    rounded_square([h_outer,w_outer],
        [rad_out, rad_out, rad_out, rad_out],
        center=true);
    translate([0, 2]){
      rounded_square([h_inner,w_inner-4],
          [rad_in, rad_in, rad_in, rad_in],
          center=true);   
    }
  }
}

difference(){
  square([dist_x, dist_y]);
  pad_dist_x = dist_x - dist_edge*2;
  pad_dist_y = dist_y - dist_edge*2;
  translate([dist_edge, dist_edge]){
    for (i=[0:1]){
      for(j=[0:1]){
        translate([pad_dist_x*i, pad_dist_y*j]){
          sensor_pad(); 
        }
      }
    }
  }
}
