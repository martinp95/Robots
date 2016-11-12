diametro_agujero = 6.2;
altura_agujero = 17;
diametro_cilindro = 7;
altura_cilindro = 14;

translate([5,10,10])
vaciarCilindroInterior();
translate([-5,10,10])
vaciarCilindroInterior();

translate([-5,20,10])
vaciarCilindroInterior();

translate([5,20,10])
vaciarCilindroInterior();

module crearCilindroExterior(){
cylinder(h=altura_cilindro,d=diametro_cilindro,center = true,$fn=16);
}

module crearCilindroInterior(){
cylinder(h=altura_agujero,d=diametro_agujero,center = true,$fn=16);
}

module vaciarCilindroInterior(){
    difference(){
        crearCilindroExterior();
        crearCilindroInterior();
    }
}