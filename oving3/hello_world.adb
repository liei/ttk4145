with Ada.Text_IO;

use Ada.Text_IO;

procedure Hello_World is
   function Get_Input(Prompt : String) return String is
   begin
      Put(Prompt);
      return Get_Line;
   end Get_Input;
   Name : String := Get_Input("Your name: ");
begin
   Put_Line("Hello " & Name & "!");
end Hello_World;
