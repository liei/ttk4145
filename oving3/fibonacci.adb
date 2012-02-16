with Ada.TEXT_IO, Ada.Integer_Text_IO, Ada.Command_Line;
use  Ada.TEXT_IO, Ada.Integer_Text_IO, Ada.Command_Line;

procedure Fibonacci is
   function Calculate_Fibonacci (N : Natural) return Long_Long_Integer is
      F : array (0..2) of Long_Long_Integer := (0,1,1);   
   begin
      if (N <= 2) then
	return F(N);
      end if;
      for I in 3 .. N loop
	 F(0) := F(1);
	 F(1) := F(2);
	 F(2) := F(0) + F(1);
      end loop;
      return F(2);
   end Calculate_Fibonacci;
   
   N : Integer;
   F : Long_Long_Integer;
begin
   if(Argument_Count < 1) then
      Put_Line("No argument"); 
   else 
      N := Integer'Value(Argument(1));
      F := Calculate_Fibonacci(N);
      Put("Fib[");
      Put(N,0);
      Put("] = ");
      Put(Long_Long_Integer'Image(F));
   end if;
end Fibonacci;
