for f in $2/*.in;
do printf "Plik $f "; 
	./$1 < ${f} 1> ${f%in}result.out 2> ${f%in}result.err;
	if diff ${f%in}result.out ${f%in}out > diffout.out && diff ${f%in}result.err ${f%in}err > diffout.out
		then printf "OK\n";
		else printf "ERROR\n";
	fi;
done
