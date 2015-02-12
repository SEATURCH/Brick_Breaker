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
	 
	 signal saved_33: unsigned(31 downto 0);
	 signal saved_34: unsigned(31 downto 0);
	 signal saved_35: unsigned(31 downto 0);
	 signal saved_36: unsigned(31 downto 0);
	 signal saved_37: unsigned(31 downto 0);
	 signal saved_38: unsigned(31 downto 0);
	 signal saved_39: unsigned(31 downto 0);
	 signal saved_40: unsigned(31 downto 0);
	 signal saved_41: unsigned(31 downto 0);
	 signal saved_42: unsigned(31 downto 0);
	 signal saved_43: unsigned(31 downto 0);
	 signal saved_44: unsigned(31 downto 0);
	 signal saved_45: unsigned(31 downto 0);
	 signal saved_46: unsigned(31 downto 0);
	 signal saved_47: unsigned(31 downto 0);
	 signal saved_48: unsigned(31 downto 0);
	 signal saved_49: unsigned(31 downto 0);
	 signal saved_50: unsigned(31 downto 0);
	 signal saved_51: unsigned(31 downto 0);
	 signal saved_52: unsigned(31 downto 0);
	 signal saved_53: unsigned(31 downto 0);
	 signal saved_54: unsigned(31 downto 0);
	 signal saved_55: unsigned(31 downto 0);
	 signal saved_56: unsigned(31 downto 0);
	 signal saved_57: unsigned(31 downto 0);
	 signal saved_58: unsigned(31 downto 0);
	 signal saved_59: unsigned(31 downto 0);
	 signal saved_60: unsigned(31 downto 0);
	 signal saved_61: unsigned(31 downto 0);
	 signal saved_62: unsigned(31 downto 0);
	 signal saved_63: unsigned(31 downto 0);
	 signal saved_64: unsigned(31 downto 0);
	 signal state: unsigned(5 downto 0):= "000000";
begin
    
    --saved_value
    process (clk,saved_value, saved_1, saved_2, saved_3, saved_4, saved_5, saved_6, saved_7, saved_8, saved_9, saved_10, saved_11, saved_12, saved_13, saved_14, saved_15, saved_16, saved_17, saved_18, saved_19, saved_20, saved_21, saved_22, saved_23, saved_24, saved_25, saved_26, saved_27, saved_28, saved_29, saved_30, saved_31, saved_32, 
	 saved_33, saved_34, saved_35, saved_36, saved_37, saved_38, saved_39, saved_40, saved_41, saved_42, saved_43, saved_44, saved_45, saved_46, saved_47, saved_48, saved_49, saved_50, saved_51, saved_52, saved_53, saved_54, saved_55, saved_56, saved_57, saved_58, saved_59, saved_60, saved_61, saved_62, saved_63, saved_64)
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
						
						saved_33 <= "00000000000000000000000000000000";
						saved_34 <= "00000000000000000000000000000000";
						saved_35 <= "00000000000000000000000000000000";
						saved_36 <= "00000000000000000000000000000000";
						saved_37 <= "00000000000000000000000000000000";
						saved_38 <= "00000000000000000000000000000000";
						saved_39 <= "00000000000000000000000000000000";
						saved_40 <= "00000000000000000000000000000000";
						saved_41 <= "00000000000000000000000000000000";
						saved_42 <= "00000000000000000000000000000000";
						saved_43 <= "00000000000000000000000000000000";
						saved_44 <= "00000000000000000000000000000000";
						saved_45 <= "00000000000000000000000000000000";
						saved_46 <= "00000000000000000000000000000000";
						saved_47 <= "00000000000000000000000000000000";
						saved_48 <= "00000000000000000000000000000000";
						saved_49 <= "00000000000000000000000000000000";
						saved_50 <= "00000000000000000000000000000000";
						saved_51 <= "00000000000000000000000000000000";
						saved_52 <= "00000000000000000000000000000000";
						saved_53 <= "00000000000000000000000000000000";
						saved_54 <= "00000000000000000000000000000000";
						saved_55 <= "00000000000000000000000000000000";
						saved_56 <= "00000000000000000000000000000000";
						saved_57 <= "00000000000000000000000000000000";
						saved_58 <= "00000000000000000000000000000000";
						saved_59 <= "00000000000000000000000000000000";
						saved_60 <= "00000000000000000000000000000000";
						saved_61 <= "00000000000000000000000000000000";
						saved_62 <= "00000000000000000000000000000000";
						saved_63 <= "00000000000000000000000000000000";
						saved_64 <= "00000000000000000000000000000000";
						state <= "00000";
            elsif (wr_en = '1' and addr = "01") then
					case state is
						when "000000" => saved_1 <= unsigned(writedata);
						when "000001" => saved_2 <= unsigned(writedata);
						when "000010" => saved_3 <= unsigned(writedata);
						when "000011" => saved_4 <= unsigned(writedata);
						when "000100" => saved_5 <= unsigned(writedata);
						when "000101" => saved_6 <= unsigned(writedata);
						when "000110" => saved_7 <= unsigned(writedata);
						when "000111" => saved_8 <= unsigned(writedata);
						when "001000" => saved_9 <= unsigned(writedata);
						when "001001" => saved_10 <= unsigned(writedata);
						when "001010" => saved_11 <= unsigned(writedata);
						when "001011" => saved_12 <= unsigned(writedata);
						when "001100" => saved_13 <= unsigned(writedata);
						when "001101" => saved_14 <= unsigned(writedata);
						when "001110" => saved_15 <= unsigned(writedata);
						when "001111" => saved_16 <= unsigned(writedata);
						when "010000" => saved_17 <= unsigned(writedata);
						when "010001" => saved_18 <= unsigned(writedata);
						when "010010" => saved_19 <= unsigned(writedata);
						when "010011" => saved_20 <= unsigned(writedata);
						when "010100" => saved_21 <= unsigned(writedata);
						when "010101" => saved_22 <= unsigned(writedata);
						when "010110" => saved_23 <= unsigned(writedata);
						when "010111" => saved_24 <= unsigned(writedata);
						when "011000" => saved_25 <= unsigned(writedata);
						when "011001" => saved_26 <= unsigned(writedata);
						when "011010" => saved_27 <= unsigned(writedata);
						when "011011" => saved_28 <= unsigned(writedata);
						when "011100" => saved_29 <= unsigned(writedata);
						when "011101" => saved_30 <= unsigned(writedata);
						when "011110" => saved_31 <= unsigned(writedata);
						when "011111" => saved_32 <= unsigned(writedata);
		
						when "100000" => saved_33 <= unsigned(writedata);
						when "100001" => saved_34 <= unsigned(writedata);
						when "100010" => saved_35 <= unsigned(writedata);
						when "100011" => saved_4 <= unsigned(writedata);
						when "100100" => saved_5 <= unsigned(writedata);
						when "100101" => saved_6 <= unsigned(writedata);
						when "100110" => saved_7 <= unsigned(writedata);
						when "100111" => saved_8 <= unsigned(writedata);
						when "101000" => saved_9 <= unsigned(writedata);
						when "101001" => saved_10 <= unsigned(writedata);
						when "101010" => saved_11 <= unsigned(writedata);
						when "101011" => saved_12 <= unsigned(writedata);
						when "101100" => saved_13 <= unsigned(writedata);
						when "101101" => saved_14 <= unsigned(writedata);
						when "101110" => saved_15 <= unsigned(writedata);
						when "101111" => saved_16 <= unsigned(writedata);
						when "110000" => saved_17 <= unsigned(writedata);
						when "110001" => saved_18 <= unsigned(writedata);
						when "110010" => saved_19 <= unsigned(writedata);
						when "110011" => saved_20 <= unsigned(writedata);
						when "110100" => saved_21 <= unsigned(writedata);
						when "110101" => saved_22 <= unsigned(writedata);
						when "110110" => saved_23 <= unsigned(writedata);
						when "110111" => saved_24 <= unsigned(writedata);
						when "111000" => saved_25 <= unsigned(writedata);
						when "111001" => saved_58  <= unsigned(writedata);
						when "111010" => saved_59 <= unsigned(writedata);
						when "111011" => saved_60 <= unsigned(writedata);
						when "111100" => saved_61 <= unsigned(writedata);
						when "111101" => saved_62 <= unsigned(writedata);
						when "111110" => saved_63 <= unsigned(writedata);
						when "111111" => saved_64 <= unsigned(writedata);		
					end case;
						saved_value <= std_logic_vector( (saved_1+ saved_2+ saved_3+ saved_4+ saved_5+ saved_6+ saved_7+ saved_8 + saved_9+ saved_10+ saved_11+ saved_12+ saved_13+ saved_14+ saved_15+ saved_16+	saved_17+ saved_18+ saved_19+ saved_20+ saved_21+ saved_22+ saved_23+ saved_24 + saved_25+ saved_26+ saved_27+ saved_28+ saved_29+ saved_30+ saved_31+ saved_32+
						 saved_33+ saved_34+ saved_35+ saved_36+ saved_37+ saved_38+ saved_39+ saved_40+ saved_41+ saved_42+ saved_43+ saved_44+ saved_45+ saved_46+ saved_47+ saved_48+ saved_49+ saved_50+ saved_51+ saved_52+ saved_53+ saved_54+ saved_55+ saved_56+ saved_57+ saved_58+ saved_59+ saved_60+ saved_61+ saved_62+ saved_63+ saved_64) srl 6 );  
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
