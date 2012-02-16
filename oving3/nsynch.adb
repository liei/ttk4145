with Ada.Text_IO;

procedure NSynch is

   N : constant := 10;

   protected Manager is
      entry Synchronize;
   private
      --  Fill in
   end Manager;

   protected body Manager is
      --  Fill in
   end Manager;

   task type Worker;

   task body Worker is
   begin
      --  Fill in
   end Worker;

   Workers : array (1 .. N) of Worker;

begin
   null;
end NSynch;






