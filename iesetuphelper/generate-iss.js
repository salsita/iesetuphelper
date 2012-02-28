/**
 * Script to generate InnoSetup dll function definitions from .cpp files.
 */

var fso = WScript.CreateObject("Scripting.FileSystemObject");
var dllName = 'iesetuphelper.dll';
var outFile;
var rxVoid = /void/;
var rxString = /const wchar_t \*/;
var rxHwnd = /HWND/;
var rxBool = /BOOL|bool/;

function parseParam(p)
{
  var rxPar = /^\s*(.*[^a-zA-Z0-9])([a-zA-Z0-9]+)\s*$/;
  var parDef = p.match(rxPar);
  if (parDef && parDef.length > 2)
  {
    var pp = {};
    pp.type = parDef[1];
    pp.name = parDef[2];
    return pp;
  } else {
    return null;
  }
}

function parseParamSpec(paramSpec)
{
  var paramDefs = [];
  paramSpec = paramSpec.replace(/^\s*/, '').replace(/\s*$/, '');
  var allParams = paramSpec.split(',');
  for (var i = 0; i < allParams.length; i ++)
  {
    var pp = parseParam(allParams[i]);
    if (pp)
    {
      paramDefs.push(pp);
    }
  }
  return paramDefs;
}

function dumpExport(funcOrProc, name, paramList, retType, externalSpec)
{
  var pascalFuncDef = funcOrProc + ' ';
  pascalFuncDef += name;
  pascalFuncDef += '(' + paramList + ')';
  pascalFuncDef += retType;
  pascalFuncDef += '; ';
  pascalFuncDef += externalSpec;
  outFile.WriteLine(pascalFuncDef);
}

function translateType(cType)
{
  if (cType.match(rxVoid))
    return null;

  if (cType.match(rxString))
    return 'String';

  if (cType.match(rxHwnd))
    return 'longint';

  if (cType.match(rxBool))
    return 'Boolean';

  throw 'Type "' + cType + '" not recognized.';
}

function processExport(retVal, name, paramSpec)
{
  var retType = translateType(retVal);
  var paramDefs = parseParamSpec(paramSpec);
  var pascalParamSpec = '';
  for (var i = 0; i < paramDefs.length; i ++)
  {
    if (pascalParamSpec != '')
    {
      pascalParamSpec += '; ';
    }
    var t = translateType(paramDefs[i].type);
    var n = paramDefs[i].name.charAt(0).toUpperCase() + paramDefs[i].name.slice(1);
    pascalParamSpec += n + ': ' + t;
  }

  var funcOrProc = retType ? 'function' : 'procedure';
  var funcRetType = retType ? (': ' + retType) : '';

  dumpExport(funcOrProc, name, pascalParamSpec, funcRetType, 'external \'' + name + '@files:' + dllName + ' stdcall delayload\';');
  dumpExport(funcOrProc, name + 'Uninst', pascalParamSpec, funcRetType, 'external \'' + name + '@{#iesetuphelerInstallDir}\\' + dllName + ' stdcall uninstallonly delayload\';');
  outFile.WriteLine('');
}

function parseCppFile(fileName)
{
  var rxExport = /[\s]*EXPORTED\(\s*([^,]+)\s*,\s*([^,]+)\s*\)\s*\(\s*(.*)\s*\)/;
  var f = fso.OpenTextFile(fileName, 1);
  while (!f.AtEndOfStream)
  {
    var l = f.ReadLine();
    var exportDefinition = l.match(rxExport);
    if (exportDefinition && exportDefinition.length > 3)
    {
      processExport(exportDefinition[1], exportDefinition[2], exportDefinition[3]);
    }
  }
  f.Close();
}

var dir = fso.GetFolder('.');
outFile = fso.CreateTextFile('helpercode.isi', true);

outFile.WriteLine('#ifndef iesetuphelerInstallDir');
outFile.WriteLine('#define iesetuphelerInstallDir "{app}"');
outFile.WriteLine('#endif');
outFile.WriteLine('');

for (var fileEnum = new Enumerator(dir.files); !fileEnum.atEnd(); fileEnum.moveNext())
{
  if (fileEnum.item().name.match(/\.cpp$/i))
  {
    parseCppFile(fileEnum.item());
  }
}
outFile.Close();
