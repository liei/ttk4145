with Ada.Text_IO;

use Ada.Text_IO;

procedure Hello_World is
begin
   Put("Your name: ");
   declare
      Name : String := Get_Line;
   begin 
      Put_Line("Hello " & Name & "!");
   end;
end Hello_World;
