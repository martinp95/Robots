diametro_cilindro = 95;
altura_cilindro = 2;

diametro_agujeros_cilindro_grandes = 5;
altura_agujeros_cilindro = 20;
diametro_agujeros_cilindro_pequenos = 4;

creacion_cilindros_perforados();
unir_piezas_hacer_agujeros();



//creacion de los cilindros con agujeros.
module creacion_cilindros_perforados(){
    pieza_cilindro1();
    pieza_cilindro2();
}

module pieza_cilindro2(){
    difference(){
    creacion_cilindro2();
     agujeros_cilindro2();
    }
}
module agujeros_cilindro2(){
    translate([-51,-9,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-51,-91,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-92,-50,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}
translate([-10,-50,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-38,-50,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}
translate([-65,-50,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-51,-64,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-51,-37,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}
}


module pieza_cilindro1(){
    difference(){
        creacion_cilindro1();
        agujeros_grandes_cilindro1();
        agujeros_pequenos_cilindro1();
    }
}

module creacion_cilindro1(){
translate([-47.5,51.5,1]){
color("red")
cylinder(h=altura_cilindro,d=diametro_cilindro,center = true);
}
}

module creacion_cilindro2(){
    translate([-51.5,-50.5,1]){
color("red")
cylinder(h=altura_cilindro,d=diametro_cilindro,center = true);
}
}


module agujeros_grandes_cilindro1(){
    translate([-49,11,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}
   translate([-49,93,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-7,51,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-89,51,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-62,51,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-34,51,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-49,66,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
}

translate([-49,37,0]){
color("green")
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_grandes,center = true);
} 
}


module agujeros_pequenos_cilindro1(){
        translate([-49,20,0]){
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_pequenos,center = true);
}

translate([-49,83,0]){
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_pequenos,center = true);
}
translate([-70,30,0]){

cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_pequenos,center = true);
}

translate([-26,30,0]){

cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_pequenos,center = true);
}

translate([-70,74,0]){

cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_pequenos,center = true);
}

translate([-26,74,0]){

cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_pequenos,center = true);
}

translate([-16,51,0]){

cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_pequenos,center = true);
}

translate([-79,51,0]){
cylinder(h=altura_agujeros_cilindro,d=diametro_agujeros_cilindro_pequenos,center = true);
}
}

//Piezas no cilindros agujeros
module unir_piezas_hacer_agujeros(){
    difference(){
        union(){
            crear_cubo();
            crear_piezas_unir();
        }
        crear_agujeros_pieza();
    }
    rotate(0,0,180)
    translate([-70,0,0])
    difference(){
        union(){
            crear_cubo();
            crear_piezas_unir();
        }
        crear_agujeros_pieza();
    }
    translate([-35,0,0])
    difference(){
        union(){
            crear_cubo();
            crear_piezas_unir();
        }
        crear_agujeros_pieza();
    }    
     translate([35,0,0])
    difference(){
        union(){
            crear_cubo();
            crear_piezas_unir();
        }
        crear_agujeros_pieza();
    }
}

module crear_cubo(){
translate([84,0,5])
color("red")
cube([10,180,10],center=true);
}

module crear_agujeros_pieza(){
translate([84,76,0])
color("blue")
cylinder(h=40,d=8,center = true,$fn = 15);

translate([84,-80,0])
color("blue")
cylinder(h=40,d=5,center = true,$fn = 15);

translate([84,-53,0])
color("blue")
cylinder(h=40,d=5,center = true,$fn = 15);

translate([84,65,0])
color("blue")
cylinder(h=40,d=3,center = true,$fn = 15);

translate([84,87,0])
color("blue")
cylinder(h=40,d=3,center = true,$fn = 15);
    
translate([94,77,0])
color("blue")
cylinder(h=40,d=3,center = true,$fn = 15);

translate([75,77,0])
color("blue")
cylinder(h=40,d=3,center = true,$fn = 15);
}

module crear_piezas_unir(){
    translate([84,54.5,26])
color("blue")
cylinder(h=32,d1=10,d2 = 1,center = true,$fn = 15);

translate([84,76.9,1])
color("blue")
rotate([0,0,90])
cylinder(h=2,d=33,center = true,$fn = 6);
}