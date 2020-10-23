import os
import sys
import re
import urllib.request
import subprocess

def DownloadFile (url, path):
	print ('Downloading ' + url)
	urllib.request.urlretrieve (url, path)

def UnzipFile (zipFile, targetFolder):
	print ('Unzipping ' + zipFile)
	subprocess.call (['7z', 'x', zipFile, '-o' + targetFolder])

def CmakeProject (projectPath, buildFolderName):
	print ('CmakeProject ' + projectPath)
	buildFolder = os.path.join (projectPath, buildFolderName)
	os.makedirs (buildFolder)
	prevWorkDir = os.getcwd ()
	os.chdir (buildFolder)
	subprocess.call (['cmake', '..', '-G', 'Visual Studio 15 2017 Win64'])
	os.chdir (prevWorkDir)

def BuildSolution (msBuildPath, solutionPath, configuration):
	print ('Building ' + solutionPath)
	subprocess.call ([msBuildPath, solutionPath, '/property:Configuration=' + configuration, '/property:Platform=x64'])

def InstallwxWidgets (targetFolder, msBuildPath, msBuildConfiguration):
	wxWidgetsName = 'wxWidgets-3.1.2'
	wxWidgetsZipUrl = 'https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.2/wxWidgets-3.1.2.7z'
	wxWidgetsZipPath = os.path.join (targetFolder, wxWidgetsName + '.7z')
	wxWidgetsFolderPath = os.path.join (targetFolder, wxWidgetsName)
	wxWidgetsIncludeFolderPath = os.path.join (wxWidgetsFolderPath, 'include')
	wxWidgetsLibFolderPath = os.path.join (wxWidgetsFolderPath, 'lib', 'vc_x64_lib')
	if not os.path.exists (wxWidgetsFolderPath):
		DownloadFile (wxWidgetsZipUrl, wxWidgetsZipPath)
		UnzipFile (wxWidgetsZipPath, wxWidgetsFolderPath)
		solutionPath = os.path.join (wxWidgetsFolderPath, 'build', 'msw', 'wx_vc15.sln')
		BuildSolution (msBuildPath, solutionPath, msBuildConfiguration)

def InstallVisualScriptEngine (targetFolder, msBuildPath, msBuildConfiguration):
	vseName = 'VisualScriptEngine-master'
	vseZipUrl = 'https://github.com/kovacsv/VisualScriptEngine/archive/master.zip'
	vseZipPath = os.path.join (targetFolder, vseName + '.zip')
	vseFolderPath = os.path.join (targetFolder, vseName)
	if not os.path.exists (vseFolderPath):
		DownloadFile (vseZipUrl, vseZipPath)
		UnzipFile (vseZipPath, targetFolder)
		CmakeProject (vseFolderPath, 'Build')
		solutionPath = os.path.join (vseFolderPath, 'Build', 'VisualScriptEngine.sln')
		installProjectPath = os.path.join (vseFolderPath, 'Build', 'INSTALL.vcxproj')
		BuildSolution (msBuildPath, solutionPath, msBuildConfiguration)
		BuildSolution (msBuildPath, installProjectPath, msBuildConfiguration)

def Main (argv):
	if len (argv) != 4:
		print ('usage: installdepswin.py <targetFolder> <msBuildPath> <msBuildConfiguration>')
		return 1
	
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	targetFolder = sys.argv[1]
	msBuildPath = sys.argv[2] # "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe"
	msBuildConfiguration = sys.argv[3]

	if not os.path.exists (targetFolder):
		os.makedirs (targetFolder)
	
	InstallwxWidgets (targetFolder, msBuildPath, msBuildConfiguration)
	InstallVisualScriptEngine (targetFolder, msBuildPath, msBuildConfiguration)
	return 0
	
sys.exit (Main (sys.argv))
