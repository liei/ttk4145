with Ada.Text_IO;
use Ada.Text_IO;

procedure Tasks is
   
   task type Printer (Message : access String; Period : access Duration);   

   task body Printer is
   begin
        loop
	 Put_Line(Message.all);
	 delay Period.all;
      end loop;
   end Printer;
   
   Hello_Message : aliased String := "Hello";
   Hello_Period : aliased Duration := 1.0;
   World_Message : aliased String := "World";
   World_Period : aliased Duration := 2.0;
   
   Hello_Printer : Printer (Message => Hello_Message'Access,Period => Hello_Period'Access);
   World_Printer : Printer (Message => World_Message'Access,Period => World_Period'Access);
begin
   null;
end Tasks;
