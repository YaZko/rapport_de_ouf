
name=$1
dl_path=~/Downloads/
if [ -z $name ] || [ ! -f ${dl_path}/${name}.pdf ]; then
	echo "Name not accepted"
else
	echo "Do you really want to update $name? (y/n)"
	read confirm
	if [ $confirm = "y" ]; then
		rm -f images/${name}.pdf
		rm -f xml_images/${name}.xml
		mv ${dl_path}${name}.pdf images/
		mv ${dl_path}${name}.xml xml_images/
	else
		exit 0
	fi
fi
