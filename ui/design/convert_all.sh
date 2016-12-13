
for f in *\ *.bmp; do mv "$f" "${f// /_}"; done
for f in *-*.bmp; do mv "$f" "${f//-/_}"; done

OUTFILE=screens_bits.h
OUTFILE_PUBLIC=screens.h

# Private include file with the screen bits
cat > ${OUTFILE}<<HERE
#pragma arm section rodata = ".text"

struct screen {
	int x;
	int y;
	int w;
	int h;
	unsigned char *bits;
};

HERE

i=0
for f in *.bmp;
do
	out="${f/.bmp/.xbm}"
	convert -monochrome -gravity center -background white -extent 200x200 "$f" "$out"
	cat "${out}" >> ${OUTFILE}
	rm -f "${out}"
	i=$((i+1))
done

cat >> ${OUTFILE}<<HERE

struct screen screens[NUM_SCREENS] = {
HERE

for f in *.bmp;
do
	name="${f/.bmp/}"
	cat >> ${OUTFILE}<<HERE
	{ 
		.x = 0,
		.y = 0,
		.w = ${name}_width,
		.h = ${name}_height,
		.bits = (unsigned char *) ${name}_bits,
	},
HERE
done
echo '};' >> ${OUTFILE}

sed -i -e 's/^static char/static const unsigned char/' ${OUTFILE}

# Public interface to screens
echo "#define NUM_SCREENS $i" > ${OUTFILE_PUBLIC}
echo -n "enum screen_index { SCREEN0 = 0, " >> ${OUTFILE_PUBLIC}

for j in `seq 1 $i`
do
    echo -n "SCREEN$j, " >> ${OUTFILE_PUBLIC}
done

echo '};' >> ${OUTFILE_PUBLIC}
echo "void display_screen(enum screen_index si);" >> ${OUTFILE_PUBLIC}

