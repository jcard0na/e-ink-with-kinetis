
#for f in *\ *.png; do mv "$f" "${f// /_}"; done
#for f in *-*.png; do mv "$f" "${f//-/_}"; done

OUTFILE=screens_bits.h

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
for f in *.png;
do
	out="${f/.png/.xbm}"
	convert -monochrome -gravity center -background white -extent 200x200 "$f" "$out"
	cat $out >> ${OUTFILE}
	rm -f $out
	i=$((i+1))
done

cat >> ${OUTFILE}<<HERE

#define NUM_SCREENS $i
struct screen screens[NUM_SCREENS] = {
HERE

for f in *.png;
do
	name="${f/.png/}"
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
