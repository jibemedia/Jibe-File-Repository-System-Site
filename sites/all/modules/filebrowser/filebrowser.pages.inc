<?php

/* This file is part of "filebrowser".
 *    Copyright 2010, arNuméral
 *    Author : Yoran Brault
 *    eMail  : yoran.brault@bad_arnumeral.fr (remove bad_ before sending an email)
 *    Site   : http://www.arnumeral.fr
 *
 * "filebrowser" is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * "filebrowser" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with "filebrowser"; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

/**
 * Callback for filebrowser_download/%node menu.
 */
function filebrowser_page_download($fid) {
  $cleanup = NULL;
  $files_fid = NULL;
  if (strpos($fid, ':')) {
    list($fid, $files_fid) = explode(':', $fid);
    $files_fid = explode(',', $files_fid);
  }
  $content = _filebrowser_node_content_load($fid);
  $node = NULL;
  $target = NULL;
  if ($content) {
    $node = node_load($content['nid']);
    $target = _filebrowser_encoding_to_fs($node, _filebrowser_get_node_root($node) . $content['path']);
  }
  if (!$content || !$node || !_filebrowser_can_download_file($node) || !$target) {
    drupal_access_denied();
    exit();
  }
  
  if (is_dir($target)) {
    if (!function_exists("zip_open")) {
      drupal_set_message(t("No ZIP support found in PHP installation, please contact your administrator"));
      return;
    }
    $zip = new ZipArchive();
    $target = file_directory_temp() . "/filebrowser_" . _filebrowser_safe_basename($target) . ".zip";
    $cleanup = $target;
    if (!file_exists(_filebrowser_safe_dirname($target))) {
      mkdir(_filebrowser_safe_dirname($target), 0777, TRUE);
    }
    if (file_exists($target)) {
      unlink($target);
    }
    _filebrowser_load_files($node, $fid);
    
    if ($zip->open($target, ZIPARCHIVE::CREATE) === TRUE) {
      foreach ($node->file_listing as $file_name => $file_data) {
        if (($file_data['kind'] === 0) && (!$files_fid || in_array($file_data['fid'], $files_fid))) {
          $fs_filename = realpath(_filebrowser_encoding_to_fs($node, _filebrowser_get_node_root($node) . "/" . $file_data['relative-path']));
          $zip->addFile($fs_filename, $file_name);
        }
      }
      $zip->close();
    }
    else {
      return t("Unable to create temporary zip file '@file'", array(
          file => $target 
      ));
    }
  }
  
  $decoded_file = _filebrowser_encoding_from_fs($node, $target);
  
  $result = module_invoke_all('filebrowser_download_manager_process', 'private', $target, $decoded_file);
  if ($result) {
    if ($cleanup && file_exists($cleanup)) {
      unlink($cleanup);
    }
  }
  else {
    drupal_access_denied();
  }
  exit();
}

function filebrowser_form_metadata($form_state, $fid) {
  $content = _filebrowser_node_content_load($fid);
  $node = node_load($content['nid']);	
  _filebrowser_load_files($node, $fid);
  $form['#node'] = $node;
  foreach ($node->file_listing as $name => &$file) {
    if ($name != '.') {
      $form[$file['fid']] = array(
          '#type' => 'fieldset', 
          '#title' => $file['display-name'], 
          '#tree' => TRUE, 
          '#collapsible' => FALSE, 
          '#collapsed' => TRUE, 
          '#theme' => 'dir_listing_metadata_group' 
      );
      
      $form[$file['fid']]['thumbnail'] = array(
          '#type' => 'markup', 
          '#value' => _filebrowser_thumbnails_generate($node, $file) 
      );
      foreach (_filebrowser_externals('metadata_info') as $name => $metadata) {
        if (isset($metadata['writable']) && $metadata['writable']) {
          $form[$file['fid']][$name] = array(
              '#type' => 'textarea', 
              '#title' => $metadata['title'], 
              '#default_value' => $file[$name] 
          );
        }
      }
    }
  }
  $form['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('save') 
  );
  return $form;
}

function filebrowser_form_metadata_submit($form, $form_state) {
  foreach ($form['#node']->file_listing as $name => $file) {
    if (isset($file['fid']) && isset($form_state['values'][$file['fid']])) {
      module_invoke_all('filebrowser_metadata_set', $file, $form_state['values'][$file['fid']]);
    }
  }
//  drupal_goto("node/{$form['#node']->nid}/{$form['#node']->file_listing['.']['fid']}");
}

function filebrowser_page_delete($fids) {
    $files_fid = explode(',', $fids);
	return drupal_get_form('filebrowser_form_delete_confirm',$files_fid);	
}

function filebrowser_update_thumbnails($fid) {
  $content = _filebrowser_node_content_load($fid);
  $node = node_load($content['nid']);	
  _filebrowser_load_files($node, $fid);
  $form['#node'] = $node;
  $batch = array(
    'operations' => array(),
    'title' => t('Processing Thumbnail Updating'),
    'init_message' => t('Thumbnail updating is starting.'),
    'progress_message' => t('Processed @current out of @total thumbnauls.'),
    'error_message' => t('Thumbnail updating has encountered an error.')
  );
  $thumbnailers = module_implements("filebrowser_thumbnailer_prepare");
  foreach ($node->file_listing as $name => &$file) {
    foreach ($thumbnailers as $thumbnailer) {
      if ($node->file_handlers->$thumbnailer->enabled_thumbnailer) {
        $operations=module_invoke($thumbnailer, "filebrowser_thumbnailer_prepare", $file, $node->file_handlers->$thumbnailer);
        if ($operations && count($operations)) {
          $batch['operations'] = array_merge($batch['operations'], $operations);
        }
      }
    }
  }
//  echo "<pre>"; var_dump($batch); exit();
  if (count($batch['operations'])) {
    batch_set($batch);
    batch_process($_GET['destination']);
  }
  else {
    drupal_set_message("No operation needed for thumbnail updating");
  }  
}