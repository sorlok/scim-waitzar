#!/usr/bin/perl
use File::Copy;

#This script takes userconfig.in.xml and finds all necessary fonts
#  The .in.xml file contains tags like {fontembed:1:Bitstream Vera Sans}
#  ...locates Vera.ttf, and puts it in place of {fontembed:1}
#  (It also removes the original fontembed tags)

#Verify args
$numArgs = $#ARGV + 1;
if ($numArgs != 2) {
  print "Usage: config_parser.pl <infile> <outfile>\n";
  exit;
}

#Open the file, get all {fontembed} tags
@fonts = (undef, undef, undef, undef, undef, undef, undef, undef, undef, undef);
@paths = (undef, undef, undef, undef, undef, undef, undef, undef, undef, undef);
open (CFG_FILE, $ARGV[0]);
while ($line = <CFG_FILE>) {
  if ($line =~ m/{fontembed:([0-9]):([^}]*)}/i) {
    $fonts[$1] = $2;
  }
}
close (CFG_FILE);

#Figure out where these fonts are
for ($i=0; $i<@fonts; $i++) {
  if ($fonts[$i]) {
    $path_str = $_ for qx{'fc-match' $fonts[$i]};
    $path_str =~ s/:.+\n?//;
    $find_str = 'find /usr/share/fonts -name ' . $path_str;
    $path_str = $_ for qx{$find_str};
    chomp $path_str;
    $paths[$i] = $path_str;
  }
}

#Now, simply edit our output file with the correct replacements
open (IN_FILE, $ARGV[0]);
open (OUT_FILE, ">$ARGV[1]" ) or die "Cannot open file: $!";
while ($line = <IN_FILE>) {
  for ($i=0; $i<@fonts; $i++) {
    if ($fonts[$i]) {
      $match_path = "\{fontembed:" . $i . "\}";
      $match_font = "\{fontembed:" . $i . ":" . $fonts[$i] . "\}";
      $line =~ s/($match_path)/$paths[$i]/i;
      $line =~ s/($match_font)/$fonts[$i]/i;
    }
  }

  print (OUT_FILE $line);
}
close (IN_FILE);
close (OUT_FILE);





