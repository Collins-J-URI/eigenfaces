Uncompress the archive into the following folder:

/Library/Application Support/Developer/Shared/Xcode/Project Templates/

[create that folder if it does not exist]

There are two template projects:

  - Lean corresponds to the requirements to compile the Basic/Simple IO project
  
  
  - Fat loads pretty much all the source files in the library.
  
Whether you prefer to add to the Lean or remove from the Fat project is up to you.

Do not open either project template after you moved them to their target location, and if you do, make sure that you remove the "personal" files *.mode1, *.mode2, *.mode3  that get created inside the .xcodeproj folder.