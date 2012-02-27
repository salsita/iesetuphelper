/**
 * Script to generate InnoSetup dll function definitions from .cpp files.
 */

var fso = WScript.CreateObject("Scripting.FileSystemObject");
var dllName = 'iesetuphelper.dll';
var rxString = /const wchar_t \*/;

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
  }
}

function parseParamSpec(paramSpec)
{
  var paramDefs = [];
  paramSpec = paramSpec.replace(/^\s*\(/, '').replace(/\)\s*$/, '');
  var allParams = paramSpec.split(',');
  for (var i = 0; i < allParams.length; i ++)
  {
    paramDefs.push(parseParam(allParams[i]));
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

  WScript.Echo(pascalFuncDef);
}

function translateType(cType)
{
  if (cType.match(rxString))
    return 'String';

  return null;
}

function processExport(retVal, name, paramSpec)
{
  var retType = translateType(retVal);
  var paramDefs = parseParamSpec(paramSpec);
  var pascalParamSpec = '';
  for (var i = 0; i < paramDefs.length; i ++)
  {
    var t = translateType(paramDefs[i].type);
    if (!t)
    {
      throw 'Type "' + paramDefs[i].type + '" not recognized.';
    }
    if (pascalParamSpec != '')
    {
      pascalParamSpec += ', ';
    }
    var n = paramDefs[i].name.charAt(0).toUpperCase() + paramDefs[i].name.slice(1);
    pascalParamSpec += n + ': ' + t;
  }

  var funcOrProc = retType ? 'function' : 'procedure';
  var funcRetType = retType ? (': ' + retType) : '';

  dumpExport(funcOrProc, name, pascalParamSpec, funcRetType, 'external \'' + name + '@files:' + dllName + ' stdcall delayload\';');
  dumpExport(funcOrProc, name + 'Uninst', pascalParamSpec, funcRetType, 'external \'' + name + '@{app}/' + dllName + ' stdcall unisntallonly delayload\';');
}

function parseCppFile(fileName)
{
  var rxExport = /[\s]*EXPORTED\(\s*([^,]+)\s*,\s*([^,]+)\s*,(.+)\)/;
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
for (var fileEnum = new Enumerator(dir.files); !fileEnum.atEnd(); fileEnum.moveNext())
{
  if (fileEnum.item().name.match(/.cpp$/i))
  {
    parseCppFile(fileEnum.item());
  }
}