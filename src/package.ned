package networks_pro;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    parameters:
   		 double WS;
  		 double TO ;
         double PT ;
         double TD ;
         double ED ;
         double DD ;
         double LP ;
 @display("bgb=846.63,511.25");
    submodules:
        node0: Node {
            @display("p=139.06,208.59;i=abstract/person");
        }
        node1: Node {
            @display("p=640.085,208.59;i=abstract/person,red");
        }
        coordinator: Coordinator {
            @display("p=385.4825,85.89;i=block/broadcast");
        }
    connections:
        coordinator.out0 --> node0.in_coord;
        coordinator.out1 --> node1.in_coord;
        node0.out --> node1.in_node;
        node1.out --> node0.in_node;
}

