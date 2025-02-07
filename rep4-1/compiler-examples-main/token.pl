  while(<>) {
        chop;
        while ($_ ne '') {
            s=//.*$==;
            if (s/^[0-9][0-9]*//) {
                print "$& ";
            } elsif (s/^[a-zA-Z]//) {
                print "$& ";
            } elsif (s/^\s//) {
            } elsif (s/.//) {
                print "$& ";
            }
        }
        print "\n";
    }
