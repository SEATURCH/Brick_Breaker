library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity bit_flipper is
port (
    clk: in std_logic;
    reset_n: in std_logic;
    addr: in std_logic_vector(1 downto 0);
    rd_en: in std_logic;
    wr_en: in std_logic;
    readdata: out std_logic_vector(31 downto 0);
    writedata: in std_logic_vector(31 downto 0)
);
end bit_flipper;

architecture rtl of bit_flipper is
    signal saved_value: std_logic_vector(31 downto 0);
	 signal saved_1: unsigned(31 downto 0);
	 signal saved_2: unsigned(31 downto 0);
	 signal saved_3: unsigned(31 downto 0);
	 signal saved_4: unsigned(31 downto 0);
	 signal saved_5: unsigned(31 downto 0);
	 signal saved_6: unsigned(31 downto 0);
	 signal saved_7: unsigned(31 downto 0);
	 signal saved_8: unsigned(31 downto 0);
	 signal saved_9: unsigned(31 downto 0);
	 signal saved_10: unsigned(31 downto 0);
	 signal saved_11: unsigned(31 downto 0);
	 signal saved_12: unsigned(31 downto 0);
	 signal saved_13: unsigned(31 downto 0);
	 signal saved_14: unsigned(31 downto 0);
	 signal saved_15: unsigned(31 downto 0);
	 signal saved_16: unsigned(31 downto 0);
	 signal saved_17: unsigned(31 downto 0);
	 signal saved_18: unsigned(31 downto 0);
	 signal saved_19: unsigned(31 downto 0);
	 signal saved_20: unsigned(31 downto 0);
	 signal saved_21: unsigned(31 downto 0);
	 signal saved_22: unsigned(31 downto 0);
	 signal saved_23: unsigned(31 downto 0);
	 signal saved_24: unsigned(31 downto 0);
	 signal saved_25: unsigned(31 downto 0);
	 signal saved_26: unsigned(31 downto 0);
	 signal saved_27: unsigned(31 downto 0);
	 signal saved_28: unsigned(31 downto 0);
	 signal saved_29: unsigned(31 downto 0);
	 signal saved_30: unsigned(31 downto 0);
	 signal saved_31: unsigned(31 downto 0);
	 signal saved_32: unsigned(31 downto 0);
	 signal state: unsigned(4 downto 0):= "00000";
begin
    
    --saved_value
    process (clk,saved_value, saved_1, saved_2, saved_3, saved_4, saved_5, saved_6, saved_7, saved_8, saved_9, saved_10, saved_11, saved_12, saved_13, saved_14, saved_15, saved_16, saved_17, saved_18, saved_19, saved_20, saved_21, saved_22, saved_23, saved_24, saved_25, saved_26, saved_27, saved_28, saved_29, saved_30, saved_31, saved_32)
	 variable keep: unsigned(31 downto 0) := "00000000000000000000000111111111";
    begin
        if rising_edge(clk) then
            if (reset_n = '0') then
                saved_value <= (others => '0');
            elsif (wr_en = '1' and addr = "00") then
						saved_value <= "00000000000000000000000000000000";
						saved_1 <= "00000000000000000000000000000000";
						saved_2 <= "00000000000000000000000000000000";
						saved_3 <= "00000000000000000000000000000000";
						saved_4 <= "00000000000000000000000000000000";
						saved_5 <= "00000000000000000000000000000000";
						saved_6 <= "00000000000000000000000000000000";
						saved_7 <= "00000000000000000000000000000000";
						saved_8 <= "00000000000000000000000000000000";
						saved_9 <= "00000000000000000000000000000000";
						saved_10 <= "00000000000000000000000000000000";
						saved_11 <= "00000000000000000000000000000000";
						saved_12 <= "00000000000000000000000000000000";
						saved_13 <= "00000000000000000000000000000000";
						saved_14 <= "00000000000000000000000000000000";
						saved_15 <= "00000000000000000000000000000000";
						saved_16 <= "00000000000000000000000000000000";
						saved_17 <= "00000000000000000000000000000000";
						saved_18 <= "00000000000000000000000000000000";
						saved_19 <= "00000000000000000000000000000000";
						saved_20 <= "00000000000000000000000000000000";
						saved_21 <= "00000000000000000000000000000000";
						saved_22 <= "00000000000000000000000000000000";
						saved_23 <= "00000000000000000000000000000000";
						saved_24 <= "00000000000000000000000000000000";
						saved_25 <= "00000000000000000000000000000000";
						saved_26 <= "00000000000000000000000000000000";
						saved_27 <= "00000000000000000000000000000000";
						saved_28 <= "00000000000000000000000000000000";
						saved_29 <= "00000000000000000000000000000000";
						saved_30 <= "00000000000000000000000000000000";
						saved_31 <= "00000000000000000000000000000000";
						saved_32 <= "00000000000000000000000000000000";
						state <= "00000";
            elsif (wr_en = '1' and addr = "01") then
					case state is
						when "00000" => saved_1 <= unsigned(writedata);
						when "00001" => saved_2 <= unsigned(writedata);
						when "00010" => saved_3 <= unsigned(writedata);
						when "00011" => saved_4 <= unsigned(writedata);
						when "00100" => saved_5 <= unsigned(writedata);
						when "00101" => saved_6 <= unsigned(writedata);
						when "00110" => saved_7 <= unsigned(writedata);
						when "00111" => saved_8 <= unsigned(writedata);
						when "01000" => saved_9 <= unsigned(writedata);
						when "01001" => saved_10 <= unsigned(writedata);
						when "01010" => saved_11 <= unsigned(writedata);
						when "01011" => saved_12 <= unsigned(writedata);
						when "01100" => saved_13 <= unsigned(writedata);
						when "01101" => saved_14 <= unsigned(writedata);
						when "01110" => saved_15 <= unsigned(writedata);
						when "01111" => saved_16 <= unsigned(writedata);
						when "10000" => saved_17 <= unsigned(writedata);
						when "10001" => saved_18 <= unsigned(writedata);
						when "10010" => saved_19 <= unsigned(writedata);
						when "10011" => saved_20 <= unsigned(writedata);
						when "10100" => saved_21 <= unsigned(writedata);
						when "10101" => saved_22 <= unsigned(writedata);
						when "10110" => saved_23 <= unsigned(writedata);
						when "10111" => saved_24 <= unsigned(writedata);
						when "11000" => saved_25 <= unsigned(writedata);
						when "11001" => saved_26 <= unsigned(writedata);
						when "11010" => saved_27 <= unsigned(writedata);
						when "11011" => saved_28 <= unsigned(writedata);
						when "11100" => saved_29 <= unsigned(writedata);
						when "11101" => saved_30 <= unsigned(writedata);
						when "11110" => saved_31 <= unsigned(writedata);
						when others => saved_32 <= unsigned(writedata);				
					end case;
						saved_value <= std_logic_vector( (saved_1+ saved_2+ saved_3+ saved_4+ saved_5+ saved_6+ saved_7+ saved_8 + saved_9+ saved_10+ saved_11+ saved_12+ saved_13+ saved_14+ saved_15+ saved_16+	saved_17+ saved_18+ saved_19+ saved_20+ saved_21+ saved_22+ saved_23+ saved_24 + saved_25+ saved_26+ saved_27+ saved_28+ saved_29+ saved_30+ saved_31+ saved_32) srl 5 );  
						state <= state + 1;
					--saved <= (saved sll 9);
					--saved <= saved OR (keep AND unsigned(writedata(8 downto 0)));
					--saved_value <= std_logic_vector(saved(31 downto 0));
					--saved_value <= std_logic_vector( ( (keep AND saved(71 downto 63) ) + (keep AND saved(62 downto 54) ) + (keep AND saved(53 downto 45) ) + (keep AND saved(44 downto 36) ) + (keep AND saved(35 downto 27)) + (keep AND saved(26 downto 18)) + (keep AND saved(17 downto 9)) + (keep AND saved(8 downto 0)) )  ); 
					--case state is
					--		when "111" =>
					--		saved_value <= std_logic_vector(unsigned(saved_value) srl 3);
							--saved_value <= std_logic_vector(saved);
					--		when others =>
					--		saved_value <= std_logic_vector(unsigned(saved_value) + unsigned(writedata)); 
					-- end case;		
					-- state <= state + 1;
            --  saved_value <= std_logic_vector(unsigned(saved_value) + 1);
				--	 saved_value <= writedata;
            end if;
        end if;
    end process;
    
    --readdata
    process (rd_en, addr, saved_value)
    begin
        readdata <= (others => '-');
        if (rd_en = '1') then
            if (addr = "00") then
                -- bit-flip
                for i in 0 to 31 loop
                    readdata(i) <= saved_value(31-i);
                end loop;
            elsif (addr = "01") then
                readdata <= saved_value;
            elsif (addr = "10") then
                readdata <= not saved_value;
            end if;
        end if;
    end process;
end rtl;
