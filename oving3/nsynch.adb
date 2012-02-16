with Ada.Text_IO;

use Ada.Text_IO;

procedure NSynch is

   N : constant := 10;

   protected Manager is
      entry Synchronize;
   end Manager;

   protected body Manager is
      entry Synchronize when Synchronize'Count = N is
      begin
	 null;
      end Synchronize;
   end Manager;

   task type Worker;

   task body Worker is
   begin
      loop
	 Manager.Synchronize;
	 Put("!");
	 Manager.Synchronize;
	 Put(".");
      end loop;
   end Worker;

   Workers : array (1 .. N) of Worker;
begin
   null;
end NSynch;
