<?php

/**
 * Form callback.
 */
function entity_recur_admin_settings($form, &$form_state) {
  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (!$entity_info['fieldable']) {
      continue;
    }
    $form[$entity_type] = array(
      '#type' => 'fieldset',
      '#title' => $entity_info['label'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
      // Determine the available date fields on this node type
      $fields = array();
      $instances = field_info_instances($entity_type, $bundle);
      foreach ($instances as $name => $field) {
        if ($field['widget']['module'] == 'date') {
          $fields[$name] = $field['label'] . ' (' . $name . ')';
        }
      }

      $form[$entity_type][$bundle] = array(
        '#type' => 'fieldset',
        '#title' => $bundle_info['label'],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      if (!empty($fields)) {
        $form[$entity_type][$bundle]["entity_recur_enabled_{$entity_type}_{$bundle}"] = array(
          '#type' => 'checkbox',
          '#title' => t('Enable recurring'),
          '#description' => t('If checked, users with the right permissiosn can create recurring @bundle copies', array('@bundle' => $bundle_info['label'])),
          '#default_value' => variable_get("entity_recur_enabled_{$entity_type}_{$bundle}", FALSE),
        );
        $form[$entity_type][$bundle]["entity_recur_allow_past_dates_{$entity_type}_{$bundle}"] = array(
          '#type' => 'checkbox',
          '#title' => t('Allow dates in the past'),
          '#default_value' => variable_get("entity_recur_allow_past_dates_{$entity_type}_{$bundle}", FALSE),
          '#description' => t('If checked, recurrences with dates in the past will be allowed.'),
        );
        $form[$entity_type][$bundle]["entity_recur_max_span_{$entity_type}_{$bundle}"] = array(
          '#type' => 'select',
          '#title' => t('Max recurring duration'),
          '#options' => array(
            0 => t('No max'),
            '1 week' => t('1 week'),
            '2 weeks' => t('2 weeks'),
            '1 month' => t('1 month'),
            '3 months' => t('3 months'),
            '6 months' => t('6 months'),
            '1 year' => t('1 year'),
          ),
          '#default_value' => variable_get("entity_recur_max_span_{$entity_type}_{$bundle}", NULL),
          '#description' => t('Select a maximum time span that recurring will be allowed to continue to.'),
        );

        $default_field = variable_get("entity_recur_date_field_{$entity_type}_{$bundle}", FALSE);
        $form[$entity_type][$bundle]["entity_recur_date_field_{$entity_type}_{$bundle}"] = array(
          '#type' => 'select',
          '#title' => t('Date field'),
          '#options' => $fields,
          '#default_value' => $default_field && isset($fields[$default_field]) ? $default_field : NULL,
          '#description' => t('Select the date field that will be used to base the recurrences on.'),
        );
        $form[$entity_type][$bundle]["entity_recur_entity_form_form_{$entity_type}_{$bundle}"] = array(
          '#type' => 'checkbox',
          '#title' => t('Display recur options on entity add form'),
          '#default_value' => variable_get("entity_recur_entity_form_form_{$entity_type}_{$bundle}", FALSE),
          '#description' => t('If checked, recurring options will appear on the entity add form.'),
        );
      }
      else {
        $form[$entity_type][$bundle]['entity_recur_null'] = array(
          '#markup' => t('There are no date fields available for this bundle.'),
        );
      }
    }
  }
  return system_settings_form($form);
}
