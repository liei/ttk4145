with Ada.Text_IO;
use Ada.Text_IO;

package body Tasks is   
   task body Printer is
   begin
        loop
	 Put_Line(Message.all);
	 delay Period.all;
      end loop;
   end Printer;
end Tasks;
