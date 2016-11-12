diametro_esfera = 83;

diametro_esfera_vaciar = 75;

diametro_cilindro_sobrante = 100;
altura_cilindro_sobrante = 83;

altura_cilindro_base = 2;
diametro_cilindro_base = 94.5;

altura_cilindro_vaciar = 5;
diametro_cilindro_vaciar = 64;

diametro_agujerpos_base = 4;
altura_agujeros_base = 10;

union(){
quitarSobranteEsfera();
translate([0,0,-2])
vaciarBase();

}

module crearEsfera(){
    translate([0,0,-19])
sphere(d = diametro_esfera,center = true, $fn = 30);
}

module crearEsferaVaciar(){
    translate([0,0,-19])
    color("red")
sphere(d = diametro_esfera_vaciar,center = true, $fn = 30);
}

module vaciarEsfera(){
    difference(){
        crearEsfera();
        crearEsferaVaciar();
    }
}

module quitarSobranteEsfera(){
    difference(){
        vaciarEsfera();
        translate([0,0,-50])
            cylinder(h=diametro_cilindro_sobrante,d=altura_cilindro_sobrante,center = true);
    }
}


module crearBase(){
   translate([0,0,1])
        cylinder(h=altura_cilindro_base,d=diametro_cilindro_base,center = true,$fn = 25);
}

module cilindroVaciarBase(){
    cylinder(h=altura_cilindro_vaciar,d=diametro_cilindro_vaciar,center = true,$fn = 25);
}

module vaciarBase(){
    difference(){
        crearBase();
        cilindroVaciarBase();
        hacer_agujeros_base();
    }
}

module hacer_agujeros_base(){
    translate([29,29,0])
color("red")
cylinder(h=10,d=4,center = true,$fn=20);
    
    translate([-29,29,0])
color("red")
cylinder(h=10,d=4,center = true,$fn=20);
    
        translate([29,-29,0])
color("red")
cylinder(h=10,d=4,center = true,$fn=20);
    
    translate([-29,-29,0])
color("red")
cylinder(h=10,d=4,center = true,$fn=20);
}