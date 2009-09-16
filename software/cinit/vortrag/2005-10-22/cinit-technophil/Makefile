#
# LaTeX Makefile for slides
# Nico Schottelius
#

#PDFVIEWER=xpdf -z width -fullscreen 
PDFVIEWER=xpdf -z width

all: clean-pdf

public:
	scp *.pdf nico.schotteli.us/papers/

%.show: %.pdf
	$(PDFVIEWER) $<

%.pdf: %.ps
	ps2pdf $<

%.ps: %.dvi
	dvips $<

%.dvi: %.tex
	latex $< && latex $< && latex $<

all-pdf:
	for source in *.tex; do latex2pdf $${source%.tex}; done

clean-pdf: pdf unused-clean

clean:
	rm -f *.aux *.log *.dvi *.toc *.ps *.pdf
