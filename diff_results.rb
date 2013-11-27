#!/usr/bin/env ruby

require 'bigdecimal'

if ARGV.length != 2
  puts "Usage: ruby diff_results.rb <File1> <File2>"
end

f1 = File.open ARGV[0]
f2 = File.open ARGV[1]

avg_sum = BigDecimal.new(0)
count = 0

while !f1.eof? && !f2.eof? do
  r1 = f1.readline
  r2 = f2.readline
  if r1 == "\n" && r2 == "\n"
    next
  end
  r1 = r1.chop.split(" ")[2]
  r2 = r2.chop.split(" ")[2]
  d1 = BigDecimal.new(r1)
  d2 = BigDecimal.new(r2)
  diff = (d1 - d2).abs
  if (diff / d1).nan? || (diff / d1).infinite?
    # puts "#{r1} #{r2} 0 0"
    puts "0 0"
  else
    # puts "#{r1} #{r2} #{diff} #{(diff / d1).round(2).to_f}"
    puts "#{(diff / d1).round(2).to_f}"
    avg_sum += (diff / d1)
  end
  count += 1
end

puts "Average error #{(avg_sum / count).to_f}"


