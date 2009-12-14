<?php


function cckbmlabels_preprocess_content_display_overview_form(&$vars) { 
  $form = &$vars['form'];

  $contexts_selector = $form['#contexts'];
  $vars['basic'] = $contexts_selector == 'basic';
  $vars['contexts'] = content_build_modes($contexts_selector);

  if ($contexts_selector == 'basic') {
    $help = t("Configure how this content type's fields and field labels should be displayed when it's viewed in teaser and full-page mode.");
  }
  else {
    $help = t("Configure how this content type's fields should be displayed when it's rendered in the following contexts.");
  }
  $help .= ' '. t("Use the 'Exclude' checkbox to exclude an item from the !content value passed to the node template.", array('!content' => '$content'));
  $vars['help'] = $help;

  $order = _content_overview_order($form, $form['#fields'], $form['#groups']);
  if (empty($order)) {
    $vars['rows'] = array();
    $vars['submit'] = '';
    return;
  }

  $rows = array();
  foreach ($order as $key) {
    $element = &$form[$key];
    $row = new stdClass();
    foreach (element_children($element) as $child) {
      if (!array_key_exists('exclude', $element[$child])) {
        $row->{$child} = drupal_render($element[$child]);
      }
      else {
        $row->{$child}->label = drupal_render($element[$child]['label']);
        $row->{$child}->format = drupal_render($element[$child]['format']);
        $row->{$child}->exclude = drupal_render($element[$child]['exclude']);
      }
    }
    $row->label_class = in_array($key, $form['#groups']) ? 'label-group' : 'label-field';
    $row->indentation = theme('indentation', isset($element['#depth']) ? $element['#depth'] : 0);
    $rows[] = $row;
  }
  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
}