import subprocess

def RunConan(build_type):
    subprocess.run((
        'conan', 'install', '.', 
        '--build', 'missing', 
        '--output-folder=./.conan', 
        '--settings=compiler.cppstd=20',
        f'--settings=build_type={build_type}'
    ))

if __name__ == "__main__":
    RunConan("Debug")
    RunConan("Release")
