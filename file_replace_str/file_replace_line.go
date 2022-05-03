package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"os"
)

func main() {
	args := os.Args
	inputFile := ""
	if len(args) > 1 {
		inputFile = args[1]
	} else {
		log.Fatal("usage: executable inputFileName outputFileName")
		return
	}
	outputFile := inputFile
	if len(args) > 2 {
		outputFile = args[2]
	} else {
		outputFile = inputFile + ".new"
	}
	fmt.Println("inputFile:", inputFile, " outputFile:", outputFile)

	DoReplaceFileLine(inputFile, outputFile)
}

func DoReplaceFileLine(inputFile string, outputFile string) {
	iFile, err := os.Open(inputFile)
	if err != nil {
		log.Fatal("os.Open failed. inputFile:", inputFile, " err:", err)
		return
	}
	defer iFile.Close()

	oFile, err := os.OpenFile(outputFile, os.O_TRUNC|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatal("os.OpenFile failed. err:", err)
		return
	}
	defer oFile.Close()

	dataWriter := bufio.NewWriter(oFile)

	scanner := bufio.NewScanner(iFile)
	for scanner.Scan() {
		text := scanner.Text()
		//fmt.Println(text)
		if text == "var $protobuf = require(\"protobufjs/minimal\");" {
			text = "var $protobuf = protobuf;"
		}
		dataWriter.WriteString(text + "\n")
	}
	dataWriter.Flush()

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}

func CopyFile(src, dst string) (err error) {
	sfi, err := os.Stat(src)
	if err != nil {
		return
	}
	if !sfi.Mode().IsRegular() {
		// cannot copy non-regular files (e.g., directories,
		// symlinks, devices, etc.)
		return fmt.Errorf("CopyFile: non-regular source file %s (%q)", sfi.Name(), sfi.Mode().String())
	}
	dfi, err := os.Stat(dst)
	if err != nil {
		if !os.IsNotExist(err) {
			return
		}
	} else {
		if !(dfi.Mode().IsRegular()) {
			return fmt.Errorf("CopyFile: non-regular destination file %s (%q)", dfi.Name(), dfi.Mode().String())
		}
		if os.SameFile(sfi, dfi) {
			return
		}
	}
	if err = os.Link(src, dst); err == nil {
		return
	}
	err = copyFileContents(src, dst)
	return
}

func copyFileContents(src, dst string) (err error) {
	in, err := os.Open(src)
	if err != nil {
		return
	}
	defer in.Close()
	out, err := os.Create(dst)
	defer out.Close()
	if err != nil {
		return
	}
	defer func() {
		cerr := out.Close()
		if err == nil {
			err = cerr
		}
	}()
	if _, err = io.Copy(out, in); err != nil {
		return
	}
	err = out.Sync()
	return
}
