Filebrowser Module
------------------------
by Yoran Brault, yoran.brault@_bad_arnumeral.fr (remove _bad_)


Description
-----------
This module provides an interface for administrators to expose directories in
the file system to users through a file listing not unlike that provided by
Apache.

This module is useful when Drupal is an entire website and sits in the root
directory. All requests to the website are therefore passed to Drupal's 
index.php and directory listings cannot be found even if they're enabled.
This module alleviates those problems by passing those directory listings
through Drupal, effectively.

Installation 
------------
 * Copy the module's directory to your modules directory and activate the module.
 * Go to admin/user/permissions to change module permissions.
 * Create a news File Directory Listing node
 * That's it :)