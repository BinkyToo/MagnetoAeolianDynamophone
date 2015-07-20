module heatsink(size, offset) {
	difference() {
		cube([size, 10, size], center=true);
		union(){
			for (rot=[0, 90, 180, 270]){
				rotate([0, rot, 0])
					for (dist=[(-2*offset), -offset, 0, offset, (2*offset)]){
						translate([dist, 0, 0])
      					cylinder(h=size, r=2, centre = true);
					}
         }
      }
	}
}

heatsink(50, 10);


