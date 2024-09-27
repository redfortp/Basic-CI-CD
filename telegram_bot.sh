#!/bin/bash

STAGE_TYPE=$1
TOKEN="####"
CHAT_ID="884711515"
TIME=10

if [ "$CI_JOB_STATUS" == "success" ]; then
  MESSAGE="✅ Стадия $STAGE_TYPE $CI_JOB_NAME успешно завершена %0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG"
else
  MESSAGE="🚫 Стадия $STAGE_TYPE $CI_JOB_NAME завершилась с ошибкой!  +$CI_PROJECT_URL/pipelines"
fi

URL="https://api.telegram.org/bot$TOKEN/sendMessage"

curl -s --max-time $TIME -d "chat_id=$CHAT_ID&disable_web_page_preview=1&text=$MESSAGE" $URL > /dev/null
