#/bin/sh
#arg1: Image Folder
#line_num=`cat phrasebook | wc -l`
exp_num=1
page_num=0
pic_num=1
filename=$(pwd)
while [[ -e "$1/DD_MW$exp_num" ]]; do
	cp base.svg "bij_$page_num.svg"
	c_num=0
	while [[ "$c_num" -lt "2" && "$1/DD_MW$exp_num" ]]; do
		#echo $exp_num
		note=$(sed "$pic_num""q;d" "$filename/$1/note")
		sed -i'' "0,/PATH/{s:PATH:$filename/$1/DD_MW$exp_num""_S11 D.png:}" "bij_$page_num.svg"
		sed -i'' "0,/PATH/{s:PATH:$filename/$1/DD_MW$exp_num""_S11 D.png:}" "bij_$page_num.svg"
		sed -i'' "0,/PATH/{s:TEXT:$note:}" "bij_$page_num.svg"
		pic_num=$((pic_num+1))
		note=$(sed "$pic_num""q;d" "$filename/$1/note")
		sed -i'' "0,/PATH/{s:PATH:$filename/$1/DD_MW$exp_num""_S12 D.png:}" "bij_$page_num.svg"
		sed -i'' "0,/PATH/{s:PATH:$filename/$1/DD_MW$exp_num""_S12 D.png:}" "bij_$page_num.svg"
		sed -i'' "0,/PATH/{s:TEXT:$note:}" "bij_$page_num.svg"
		pic_num=$((pic_num+1))
		note=$(sed "$pic_num""q;d" "$filename/$1/note")
		sed -i'' "0,/PATH/{s:PATH:$filename/$1/DD_MW$exp_num""_S11 Phase D.png:}" "bij_$page_num.svg"
		sed -i'' "0,/PATH/{s:PATH:$filename/$1/DD_MW$exp_num""_S11 Phase D.png:}" "bij_$page_num.svg"
		sed -i'' "0,/PATH/{s:TEXT:$note:}" "bij_$page_num.svg"
		pic_num=$((pic_num+1))
		note=$(sed "$pic_num""q;d" "$filename/$1/note")
		sed -i'' "0,/PATH/{s:PATH:$filename/$1/DD_MW$exp_num""_S12 Phase D.png:}" "bij_$page_num.svg"
		sed -i'' "0,/PATH/{s:PATH:$filename/$1/DD_MW$exp_num""_S12 Phase D.png:}" "bij_$page_num.svg"
		sed -i'' "0,/PATH/{s:TEXT:$note:}" "bij_$page_num.svg"
		exp_num=$((exp_num+1))
		c_num=$((c_num+1))
		pic_num=$((pic_num+1))
	done
	inkscape "bij_$page_num.svg" --export-pdf="bij_$page_num.pdf"
	rm "bij_$page_num.svg"
	page_num=$((page_num+1))
done
pdf_arg=""
pdf_num=0
while [[ "$pdf_num" -lt "$page_num" ]]; do
	pdf_arg="$pdf_arg"" bij_$pdf_num.pdf"
	pdf_num=$((pdf_num+1))
done
#pdf_arg="$pdf_arg"" output.pdf"
#echo	$pdf_arg
pdfunite $pdf_arg output.pdf
rm $pdf_arg
