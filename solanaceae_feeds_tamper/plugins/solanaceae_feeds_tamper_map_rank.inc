<?php
/**
 * feeds_tamper API definition for drupal role mapping
 * Allow for mapping values from an allowed itis ranking terms to Brahms terms
 */
$plugin = array(
  'form' => 'solanaceae_feeds_map_rank_form',
  'callback' => 'solanaceae_feeds_map_rank_callback',
  'name' => 'Map rank',
  'multi' => 'loop',
  'category' => 'Taxonomy',
  'description' => 'This will allow you to map Brahms rank Scratchpad rank'
);

/**
 * Form for building the Brahms to Scratchpad rank term mapping
 */
function solanaceae_feeds_map_rank_form($importer, $element_key, $settings){
  $form = $sources = array();
  $source_configs = $importer->parser->getMappingSources();
  foreach($importer->processor->config['mappings'] as $mapping){
    $sources[$mapping['source']] = isset($source_configs[$mapping['source']]) ? $source_configs[$mapping['source']]['name'] : $mapping['source'];
  }
  $field_rank_allowed_values = solanaceae_itis_allowed_values(array(
    'field_name' => 'field_rank'
  ), 2);
  $itis_field_rank_allowed_values = itis_term_allowed_values(array(
    'field_name' => 'field_rank'
  ), 2);
  if(array_diff_key($itis_field_rank_allowed_values, $field_rank_allowed_values)){
    $newRankTerms = implode(' ,', array_diff_key($itis_field_rank_allowed_values, $field_rank_allowed_values));
    drupal_set_message(t('There is a new allowed term in Itis: @newTerm. Please have admin update this module.', array(
      '@newRankTerms' => $newRankTerms
    )), 'warning');
  }
  foreach($field_rank_allowed_values as $key => $value){
    $form['rank_map_' . $key] = array(
      '#type' => 'textfield',
      '#title' => t('Patterns to map to ' . $key),
      '#default_value' => isset($settings['rank_map_' . $key]) ? $settings['rank_map_' . $key] : ''
    );
  }
  return $form;
}

/**
 * Function to process the value conversion
 * Called by the feed (via Feed Tamper) to modify data.
 */
function solanaceae_feeds_map_rank_callback($result, $item_key, $element_key, &$field, $settings){
  $allowed_values = itis_term_allowed_values(array(
    'field_name' => 'field_rank'
  ), 2);
    foreach($allowed_values as $key => $value){
      $scratch_map_array = explode(",", $settings['rank_map_' . $key]);
      $scratch_map_array = array_map('trim', $scratch_map_array);
      if(in_array($field, $scratch_map_array)){
        $field = $key;
      }
  }
}
