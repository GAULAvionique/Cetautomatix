

from chardet import UniversalDetector
import os



import os
import sys
import codecs
#from chardet.universaldetector import UniversalDetector

targetFormat = 'utf-8'
outputDir = 'converted'
detector = UniversalDetector()


def detect_encoding_(fil:str):
    detector = UniversalDetector()
    with open(fil, "rb") as f:
        for line in f:
            detector.feed(line)
            if detector.done:
                break
            
    return detector.close()

def detect_encoding(current_file):
    detector.reset()
    with open(current_file, "rb") as f:
        for line in f:
            detector.feed(line)
            if detector.done:
                break
    detector.close()
    return detector.result['encoding']

def convertFileBestGuess(filename):
   sourceFormats = ['ascii', 'iso-8859-1']
   for format_ in sourceFormats:
        try:
            with open(filename, 'rU', encoding=format_) as sourceFile:
                writeConversion(sourceFile)
                print('Done.')
                return
        except UnicodeDecodeError:
            pass

def convertFileWithDetection(fileName):
    print("Converting '" + fileName + "'...")
    format=get_encoding_type(fileName)
    try:
        with codecs.open(fileName, 'rU', format) as sourceFile:
            writeConversion(sourceFile)
            print('Done.')
            return
    except UnicodeDecodeError:
        pass

    print("Error: failed to convert '" + fileName + "'.")


def writeConversion(file):
    with open(outputDir + '/' + fileName, 'w',encoding= targetFormat) as targetFile:
        for line in file:
            targetFile.write(line)



# Source - https://stackoverflow.com/a/191403
# Posted by Dzinx, modified by community. See post 'Timeline' for change history
# Retrieved 2026-03-08, License - CC BY-SA 3.0

BLOCKSIZE = 1048576 # or some other, desired size in bytes
def convert_encoding(sourceFileName:str, targetFileName:str, original_encoding:str, new_encoding:str="UTF-8"):
    with open(sourceFileName, "r", encoding=original_encoding) as sourceFile:
        with open(targetFileName, "w", encoding=new_encoding) as targetFile:
            while True:
                contents = sourceFile.read(BLOCKSIZE)
                if not contents:
                    break
                targetFile.write(contents)

def convert_dir(source_dir:str, out_dir:str, type_only=".c"):
    for di in os.walk(source_dir):
        for f in di[2]:
            if not f.endswith(type_only):
                print(f"[skip]: \t{f}")
                continue
            # détect encoding
            source_fi = os.path.join(di[0], f)
            ec = detect_encoding(source_fi)
            new_dest = os.path.join(out_dir,di[0])
            new_dest = di[0].replace(source_dir, out_dir)
            os.makedirs(new_dest, exist_ok=True)
            new_dest_f = os.path.join(new_dest, f)
            convert_encoding(source_fi,new_dest_f,ec,"UTF-8")
            print(f"[convertion({ec})]: \t{source_fi}\tto\t{new_dest_f}")

if __name__ == "__main__":
    d = os.getcwd()
    rd = "C:/programation/ulaval/GAUL/programmation/TAGCAN_convert"
    convert_dir(d, rd, (".c",".h"))
