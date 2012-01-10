#!/usr/bin/env ruby
#
sqtocs = { "AA" => 0, "AC" => 1, "AG" => 2, "AT" => 3,
           "CA" => 1, "CC" => 0, "CG" => 3, "CT" => 2,
           "GA" => 2, "GC" => 3, "GG" => 0, "GT" => 1,
           "TA" => 3, "TC" => 2, "TG" => 1, "TT" => 0 }

STDIN.each_line do |l|
  if l =~ /^>/
    puts l
  else
    cs = ""
    (0..l.size-2).each do |i|
      if l[i].chr == 'N' || l[i+1].chr == 'N'
        cs << '.'
      else
        cs << sqtocs[l[i].chr + l[i+1].chr].to_s
      end
    end
    puts l[0] + cs
  end
end
