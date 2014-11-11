#/usr/bin/perl

open in, "<../../NetWork/proto/cs_core.proto" or die "failed to open file cs_core.proto";

open out, ">protoEnum.pkg" or die "failed to ipen file protoEnum.pkg";
$enum_start = 0;
while(<in>) {
    $line = $_;
    if(/^enum\s.*/){
		print "find enum\n";
		$enum_start = 1;
		print out "$line";
		next;
    }
    
    if(/^}$/ && $enum_start == 1){
		print "end  enum\n";
		chomp $line;
		print out "$line;\n";
		$enum_start = 0;
		next;
    }
    if($enum_start == 1) {
		if(/\s(\w+)\s*=.*/) {
			print out "$1,\n";
		
		}
	}
    
}
close in;
close out;