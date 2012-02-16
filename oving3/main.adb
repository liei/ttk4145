with Tasks;

use Tasks;

procedure Main is
   Hello_Message : aliased String := "Hello";
   Hello_Period : aliased Duration := 1.0;
   World_Message : aliased String := "World";
   World_Period : aliased Duration := 2.0;
   
   Hello_Printer : Printer (Message => Hello_Message'Access,Period => Hello_Period'Access);
   World_Printer : Printer (Message => World_Message'Access,Period => World_Period'Access);
begin
   null;
end Main;

